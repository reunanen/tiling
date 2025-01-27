#include "tiling.h"
#include <algorithm> // std::min and std::max
#include <stdexcept>
#include <string>
#include <assert.h>

namespace tiling {

int find_starting_center(int full_dimension, int tile_dimension, int overlap)
{
    assert(tile_dimension > overlap);
    const int stride = tile_dimension - overlap;
    int starting_center = full_dimension / 2;
    while (starting_center - stride > 0) {
        starting_center -= stride;
    }
    if (starting_center > tile_dimension / 2) {
        starting_center -= stride / 2;
    }
    assert(starting_center >= 0);
    assert(starting_center <= tile_dimension / 2 || overlap < 0);
    return starting_center;
}

tiles::tiles(const tiling::size& size, const tiling::parameters& parameters)
    : width (size.width)
    , height(size.height)
    , parameters(parameters)
    , starting_center_x(find_starting_center(size.width,  parameters.max_tile_width,  parameters.overlap_x))
    , starting_center_y(find_starting_center(size.height, parameters.max_tile_height, parameters.overlap_y))
    , stride_x(parameters.max_tile_width  - parameters.overlap_x)
    , stride_y(parameters.max_tile_height - parameters.overlap_y)
    , count_x((size.width  - starting_center_x - 1) / stride_x + 1)
    , count_y((size.height - starting_center_y - 1) / stride_y + 1)
{}

tiles::const_iterator::const_iterator(const tiles* parent, int center_x, int center_y, int index_x, int index_y)
    : parent(parent)
    , center_x(center_x)
    , center_y(center_y)
    , index_x(index_x)
    , index_y(index_y)
{}

const tile& tiles::const_iterator::operator*()
{
    if (!tile.has_value()) {
        update();
    }
    return tile.value();
}

const tile* tiles::const_iterator::operator->()
{
    if (!tile.has_value()) {
        update();
    }
    return &tile.value();
}

tiles::const_iterator& tiles::const_iterator::operator++() // prefix increment
{
    increment();
    return *this;
 }

tiles::const_iterator tiles::const_iterator::operator++(int) // postfix increment
{
    const auto output = *this;
    increment();
    return output;
}

bool operator ==(const tiles::const_iterator& lhs, const tiles::const_iterator& rhs)
{
    return lhs.center_x == rhs.center_x
        && lhs.center_y == rhs.center_y
        && lhs.index_x == rhs.index_x
        && lhs.index_y == rhs.index_y;
}

bool operator !=(const tiles::const_iterator& lhs, const tiles::const_iterator& rhs)
{
    return !operator ==(lhs, rhs);
}

void tiles::const_iterator::increment_x()
{
    center_x += parent->stride_x;
    ++index_x;

    assert((center_x >= parent->width) == (index_x >= parent->count_x));
}

void tiles::const_iterator::increment_y()
{
    center_y += parent->stride_y;
    ++index_y;

    assert((center_y >= parent->height) == (index_y >= parent->count_y));
}

void tiles::const_iterator::increment()
{
    increment_x();

    if (center_x >= parent->width) {
        increment_y();

        center_x = parent->starting_center_x;
        index_x = 0;
    }

    if (center_y >= parent->height) {
        *this = parent->end();
    }

    tile.reset();
}

void tiles::const_iterator::update()
{
    assert(!tile.has_value());

    const bool is_topmost_row = center_y == parent->starting_center_y;
    const bool is_bottommost_row = center_y + parent->stride_y >= parent->height;

    const bool is_leftmost_column = center_x == parent->starting_center_x;
    const bool is_rightmost_column = center_x + parent->stride_x >= parent->width;

    const auto& parameters = parent->parameters;

    const int desired_left   = center_x - parameters.max_tile_width  / 2;
    const int desired_top    = center_y - parameters.max_tile_height / 2;
    const int desired_right  = desired_left + parameters.max_tile_width;
    const int desired_bottom = desired_top  + parameters.max_tile_height;

    const int left   = parameters.limit_to_size ? std::max(0, desired_left) : desired_left;
    const int top    = parameters.limit_to_size ? std::max(0, desired_top)  : desired_top;
    const int right  = parameters.limit_to_size ? std::min(desired_right , parent->width)  : desired_right;
    const int bottom = parameters.limit_to_size ? std::min(desired_bottom, parent->height) : desired_bottom;

    auto& t = tile.emplace();

    t.index.x = index_x;
    t.index.y = index_y;

    t.full_rect.top_left.x = left;
    t.full_rect.top_left.y = top;
    t.full_rect.size.width = right - left;
    t.full_rect.size.height = bottom - top;

    t.non_overlapping_rect.top_left.x = is_leftmost_column ? left : left + (parameters.overlap_x + 1) / 2;
    t.non_overlapping_rect.top_left.y = is_topmost_row ? top : top + (parameters.overlap_y + 1) / 2;

    t.unique_rect.top_left.x = is_leftmost_column ? left : left + parameters.overlap_x;
    t.unique_rect.top_left.y = is_topmost_row ? top : top + parameters.overlap_y;

    const int non_overlapping_rect_right  = is_rightmost_column ? right  : right  - parameters.overlap_x / 2;
    const int non_overlapping_rect_bottom = is_bottommost_row   ? bottom : bottom - parameters.overlap_y / 2;

    t.non_overlapping_rect.size.width  = non_overlapping_rect_right  - t.non_overlapping_rect.top_left.x;
    t.non_overlapping_rect.size.height = non_overlapping_rect_bottom - t.non_overlapping_rect.top_left.y;

    const int unique_rect_right  = is_rightmost_column ? right  : right  - parameters.overlap_x;
    const int unique_rect_bottom = is_bottommost_row   ? bottom : bottom - parameters.overlap_y;

    t.unique_rect.size.width  = unique_rect_right  - t.unique_rect.top_left.x;
    t.unique_rect.size.height = unique_rect_bottom - t.unique_rect.top_left.y;
}

tiles::const_iterator tiles::begin() const
{
    return const_iterator(this, starting_center_x, starting_center_y, 0, 0);
}

tiles::const_iterator tiles::end() const
{
    return const_iterator(this, width, height, count_x, count_y);
}

size_t tiles::size() const
{
    return static_cast<size_t>(count_x) * static_cast<size_t>(count_y);
}

// wrapper for backward compatibility
std::vector<tile> get_tiles(const size& size, const parameters& parameters, std::function<bool()> isCancelled)
{
    const tiling::tiles tiles(size, parameters);

    std::vector<tile> output(tiles.size());

    size_t i = 0;

    for (auto j = tiles.begin(), end = tiles.end(); j != end; ++i, ++j) {
        output[i] = *j;

        if (i % 1000 == 0) {
            if (isCancelled()) {
                break;
            }
        }
    }

    assert(i == output.size() || isCancelled());

    return output;
}

}