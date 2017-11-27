#ifndef TILING_DLIB_WRAPPER_H
#define TILING_DLIB_WRAPPER_H

#include <vector>
#include <dlib/geometry/rectangle.h>
#include "tiling.h"

namespace tiling {

struct dlib_tile
{
    dlib::rectangle full_rect;
    dlib::rectangle non_overlapping_rect;
};

dlib::rectangle to_dlib_rectangle(const rectangle& rect)
{
    return dlib::rectangle(
        rect.top_left.x,
        rect.top_left.y,
        rect.top_left.x + rect.size.width - 1,
        rect.top_left.y + rect.size.height - 1
    );
}

std::vector<dlib_tile> get_tiles(int full_width, int full_height, const parameters& parameters)
{
    const std::vector<tile> tiles = get_tiles(tiling::size(full_width, full_height), parameters);

    std::vector<dlib_tile> output(tiles.size());

    for (int i = 0, end = tiles.size(); i < end; ++i) {
        output[i].full_rect = to_dlib_rectangle(tiles[i].full_rect);
        output[i].non_overlapping_rect = to_dlib_rectangle(tiles[i].non_overlapping_rect);
    }

    return output;
}

}

#endif // TILING_DLIB_WRAPPER_H