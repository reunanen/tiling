#include <vector>
#include <dlib/geometry/rectangle.h>
#include "tiling.h"

namespace tiling {

std::vector<dlib::rectangle> get_tiles(int full_width, int full_height, const parameters& parameters)
{
    const std::vector<tile> tiles = get_tiles(tiling::size(full_width, full_height), parameters);

    std::vector<dlib::rectangle> rects(tiles.size());

    for (int i = 0, end = tiles.size(); i < end; ++i) {
        const tile& tile = tiles[i];
        rects[i] =
            dlib::rectangle(
                tile.top_left.x,
                tile.top_left.y,
                tile.top_left.x + tile.size.width - 1,
                tile.top_left.y + tile.size.height - 1
            );
    }

    return rects;
}

}