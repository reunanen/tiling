#ifndef TILING_H
#define TILING_H

#include <vector>
#include <functional>
#include <optional>

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
    point index;
    rectangle full_rect;
    rectangle non_overlapping_rect;
    rectangle unique_rect; // max area not covered by the full rect of any other tile
};

struct parameters {
    int max_tile_width = 2048;
    int max_tile_height = 2048;
    int overlap_x = 227;
    int overlap_y = 227;
    bool limit_to_size = true;
    std::optional<rectangle> viewport_rect;
};

class tiles {
public:
    tiles(const size& size, const parameters& parameters);

    struct const_iterator {
        const_iterator(const tiles* parent, int center_x, int center_y, int index_x, int index_y);

        const tile& operator*() const;
        const tile* operator->() const;
        
        const_iterator& operator++();
        const_iterator operator++(int);

        friend bool operator ==(const const_iterator& lhs, const const_iterator& rhs);
        friend bool operator !=(const const_iterator& lhs, const const_iterator& rhs);

    private:
        void increment();
        void update() const;

        mutable std::optional<tile> tile;

        const tiles* parent;
        int center_x;
        int center_y;
        int index_x;
        int index_y;
    };

    const_iterator begin() const;
    const_iterator end() const;

    size_t size() const;

private:
    const int width;
    const int height;
    const parameters parameters;
    const int full_image_starting_center_x;
    const int full_image_starting_center_y;
    const int stride_x;
    const int stride_y;
    const int full_image_count_x;
    const int full_image_count_y;
    const int viewport_start_index_x;
    const int viewport_start_index_y;
    const int viewport_starting_center_x;
    const int viewport_starting_center_y;
    const int viewport_end_x;
    const int viewport_end_y;
    const int viewport_count_x;
    const int viewport_count_y;
};

// wrapper for backward compatibility
std::vector<tile> get_tiles(const size& size, const parameters& parameters, std::function<bool()> isCancelled = []() { return false; });

}

#endif // TILING_H
