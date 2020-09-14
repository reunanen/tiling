#include "tiling.h"
#include <algorithm> // std::min and std::max
#include <assert.h>

namespace tiling {

int find_starting_center(int full_dimension, int tile_dimension, int overlap)
{
    assert(tile_dimension > overlap);
    const int stride = tile_dimension - overlap;
    int starting_center = full_dimension / 2;
    while (starting_center - stride >= 0) {
        starting_center -= stride;
    }
    if (starting_center > tile_dimension / 2) {
        starting_center -= stride / 2;
    }
    assert(starting_center >= 0);
    assert(starting_center <= tile_dimension / 2);
    return starting_center;
}

std::vector<tile> get_tiles(const size& size, const parameters& parameters)
{
    const point starting_center(
        find_starting_center(size.width, parameters.max_tile_width, parameters.overlap_x),
        find_starting_center(size.height, parameters.max_tile_height, parameters.overlap_y)
    );

    std::vector<tile> tiles;

    int stride_x = parameters.max_tile_width - parameters.overlap_x;
    int stride_y = parameters.max_tile_height - parameters.overlap_y;

    for (int center_y = starting_center.y; center_y < size.height; center_y += stride_y) {

        const bool is_topmost_row = center_y == starting_center.y;
        const bool is_bottommost_row = center_y + stride_y >= size.height;

        for (int center_x = starting_center.x; center_x < size.width; center_x += stride_x) {

            const bool is_leftmost_column = center_x == starting_center.x;
            const bool is_rightmost_column = center_x + stride_x >= size.width;

            const int desired_left = center_x - parameters.max_tile_width / 2;
            const int desired_top = center_y - parameters.max_tile_height / 2;
            const int desired_right = desired_left + parameters.max_tile_width;
            const int desired_bottom = desired_top + parameters.max_tile_height;
            
            const int left   = parameters.limit_to_size ? std::max(0, desired_left)             : desired_left;
            const int top    = parameters.limit_to_size ? std::max(0, desired_top)              : desired_top;
            const int right  = parameters.limit_to_size ? std::min(desired_right, size.width)   : desired_right;
            const int bottom = parameters.limit_to_size ? std::min(desired_bottom, size.height) : desired_bottom;

            const tiling::point top_left(left, top);
            const tiling::size tile_size(right - left, bottom - top);
            
            tile t;
            t.full_rect = rectangle(top_left, tile_size);

            t.non_overlapping_rect.top_left.x = is_leftmost_column ? top_left.x : top_left.x + (parameters.overlap_x + 1) / 2;
            t.non_overlapping_rect.top_left.y = is_topmost_row ? top_left.y : top_left.y + (parameters.overlap_y + 1) / 2;

            t.unique_rect.top_left.x = is_leftmost_column ? top_left.x : top_left.x + parameters.overlap_x;
            t.unique_rect.top_left.y = is_topmost_row ? top_left.y : top_left.y + parameters.overlap_y;

            const int full_rect_right = top_left.x + tile_size.width;
            const int full_rect_bottom = top_left.y + tile_size.height;

            const int non_overlapping_rect_right = is_rightmost_column ? full_rect_right : full_rect_right - parameters.overlap_x / 2;
            const int non_overlapping_rect_bottom = is_bottommost_row ? full_rect_bottom : full_rect_bottom - parameters.overlap_y / 2;

            t.non_overlapping_rect.size.width = non_overlapping_rect_right - t.non_overlapping_rect.top_left.x;
            t.non_overlapping_rect.size.height = non_overlapping_rect_bottom - t.non_overlapping_rect.top_left.y;

            const int unique_rect_right = is_rightmost_column ? full_rect_right : full_rect_right - parameters.overlap_x;
            const int unique_rect_bottom = is_bottommost_row ? full_rect_bottom : full_rect_bottom - parameters.overlap_y;

            t.unique_rect.size.width = unique_rect_right - t.unique_rect.top_left.x;
            t.unique_rect.size.height = unique_rect_bottom - t.unique_rect.top_left.y;

            tiles.push_back(t);
        }
    }

    return tiles;
}

}