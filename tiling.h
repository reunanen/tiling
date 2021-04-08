#ifndef TILING_H
#define TILING_H

#include <vector>

namespace tiling {

struct point {
    point() {}
    point(int x, int y) : x(x), y(y) {}

    int x = -1;
    int y = -1;
};

struct size {
    size() {}
    size(int width, int height) : width(width), height(height) {}

    int width = -1;
    int height = -1;
};

struct rectangle {
    rectangle() {}
    rectangle(const point& top_left, const size& size) : top_left(top_left), size(size) {}

    point top_left;
    tiling::size size;
};

struct tile {
    rectangle full_rect;
    rectangle non_overlapping_rect;
    rectangle unique_rect; // max area not covered by the full rect of any other tile
};

struct parameters {
    int max_tile_width = 2048;
    int max_tile_height = 2048;
    int overlap_x = 227;
    int overlap_y = 227;
};

std::vector<tile> get_tiles(const size& size, const parameters& parameters);

}

#endif // TILING_H
