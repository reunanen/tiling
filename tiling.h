#include <vector>

namespace tiling {

struct point {
    point(int x, int y) : x(x), y(y) {}

    int x = 0;
    int y = 0;
};

struct size {
    size(int width, int height) : width(width), height(height) {}

    int width = 0;
    int height = 0;
};

struct tile {
    tile(const point& top_left, const size& size) : top_left(top_left), size(size) {}

    point top_left;
    size size;
};

struct parameters {
    int max_tile_width = 2048;
    int max_tile_height = 2048;
    int overlap_x = 227;
    int overlap_y = 227;
};

std::vector<tile> get_tiles(const size& size, const parameters& parameters);

}