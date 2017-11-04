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
        for (int center_x = starting_center.x; center_x < size.width; center_x += stride_x) {

            const int desired_left = center_x - parameters.max_tile_width / 2;
            const int desired_top = center_y - parameters.max_tile_height / 2;
            const int desired_right = desired_left + parameters.max_tile_width;
            const int desired_bottom = desired_top + parameters.max_tile_height;
            
            const int left = std::max(0, desired_left);
            const int top = std::max(0, desired_top);
            const int right = std::min(desired_right, size.width);
            const int bottom = std::min(desired_bottom, size.height);

            const tiling::point top_left(left, top);
            const tiling::size tile_size(right - left, bottom - top);
            
            tiles.push_back(tile(top_left, tile_size));

        }
    }

    return tiles;
}

}