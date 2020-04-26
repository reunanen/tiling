#include <vector>
#include <opencv2/core/core.hpp>
#include "tiling.h"

namespace tiling {

struct opencv_tile
{
    cv::Rect full_rect;
    cv::Rect non_overlapping_rect;
    cv::Rect unique_rect;
};

cv::Rect to_opencv_rectangle(const rectangle& rect)
{
    return cv::Rect(
        rect.top_left.x,
        rect.top_left.y,
        rect.size.width,
        rect.size.height
    );
}

std::vector<opencv_tile> get_tiles(int full_width, int full_height, const parameters& parameters)
{
    const std::vector<tile> tiles = get_tiles(tiling::size(full_width, full_height), parameters);

    std::vector<opencv_tile> output(tiles.size());

    for (int i = 0, end = tiles.size(); i < end; ++i) {
        output[i].full_rect = to_opencv_rectangle(tiles[i].full_rect);
        output[i].non_overlapping_rect = to_opencv_rectangle(tiles[i].non_overlapping_rect);
        output[i].unique_rect = to_opencv_rectangle(tiles[i].unique_rect);
    }

    return output;
}

}