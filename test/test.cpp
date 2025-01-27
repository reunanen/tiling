#include "../tiling.h"
#include "picotest/picotest.h"

namespace {

class TilingTest : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    TilingTest() {
        // You can do set-up work for each test here.
    }

    virtual ~TilingTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(TilingTest, HandlesSmallImageCorrectly) {
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(tiling::size(500, 500), parameters);
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0].index.x, 0);
    EXPECT_EQ(tiles[0].index.y, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].full_rect.size.width, 500);
    EXPECT_EQ(tiles[0].full_rect.size.height, 500);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.width, 500);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.height, 500);
    EXPECT_EQ(tiles[0].unique_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].unique_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].unique_rect.size.width, 500);
    EXPECT_EQ(tiles[0].unique_rect.size.height, 500);
}

TEST_F(TilingTest, HandlesMaxTileSizeImageCorrectly) {
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(tiling::size(parameters.max_tile_width, parameters.max_tile_height), parameters);
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0].index.x, 0);
    EXPECT_EQ(tiles[0].index.y, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].full_rect.size.width, parameters.max_tile_width);
    EXPECT_EQ(tiles[0].full_rect.size.height, parameters.max_tile_height);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.width, parameters.max_tile_width);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.height, parameters.max_tile_height);
    EXPECT_EQ(tiles[0].unique_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].unique_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].unique_rect.size.width, parameters.max_tile_width);
    EXPECT_EQ(tiles[0].unique_rect.size.height, parameters.max_tile_height);
}

void CheckStartCoordinates(const std::vector<tiling::tile>& tiles)
{
    EXPECT_EQ(tiles.front().full_rect.top_left.x, 0);
    EXPECT_EQ(tiles.front().full_rect.top_left.y, 0);
    EXPECT_EQ(tiles.front().non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles.front().non_overlapping_rect.top_left.y, 0);
    EXPECT_EQ(tiles.front().unique_rect.top_left.x, 0);
    EXPECT_EQ(tiles.front().unique_rect.top_left.y, 0);
}

void CheckEndCoordinates(const std::vector<tiling::tile>& tiles, const tiling::size& size)
{
    const auto& last_tile = tiles.back();
    EXPECT_EQ(last_tile.full_rect.top_left.x + last_tile.full_rect.size.width, size.width);
    EXPECT_EQ(last_tile.full_rect.top_left.y + last_tile.full_rect.size.height, size.height);
    EXPECT_EQ(last_tile.non_overlapping_rect.top_left.x + last_tile.non_overlapping_rect.size.width, size.width);
    EXPECT_EQ(last_tile.non_overlapping_rect.top_left.y + last_tile.non_overlapping_rect.size.height, size.height);
    EXPECT_EQ(last_tile.unique_rect.top_left.x + last_tile.unique_rect.size.width, size.width);
    EXPECT_EQ(last_tile.unique_rect.top_left.y + last_tile.unique_rect.size.height, size.height);
}

void CheckOverlap(const std::vector<tiling::tile>& tiles, const tiling::parameters& parameters, int rowCount, int columnCount)
{
    EXPECT_EQ(tiles.size(), rowCount * columnCount);

    for (int row = 0; row < rowCount - 1; ++row) {
        for (int column = 0; column < columnCount - 1; ++column) {
            const int index = row * columnCount + column;
            EXPECT_EQ(tiles[index].index.x, column);
            EXPECT_EQ(tiles[index].index.y, row);
            EXPECT_EQ(tiles[index].full_rect.top_left.x + tiles[index].full_rect.size.width - tiles[index + 1].full_rect.top_left.x, parameters.overlap_x);
            EXPECT_EQ(tiles[index].full_rect.top_left.y + tiles[index].full_rect.size.height - tiles[index + columnCount].full_rect.top_left.y, parameters.overlap_y);
            EXPECT_EQ(tiles[index].non_overlapping_rect.top_left.x + tiles[index].non_overlapping_rect.size.width, tiles[index + 1].non_overlapping_rect.top_left.x);
            EXPECT_EQ(tiles[index].non_overlapping_rect.top_left.y + tiles[index].non_overlapping_rect.size.height, tiles[index + columnCount].non_overlapping_rect.top_left.y);
            EXPECT_EQ(tiles[index].full_rect.top_left.x + tiles[index].full_rect.size.width, tiles[index + 1].unique_rect.top_left.x);
            EXPECT_EQ(tiles[index].full_rect.top_left.y + tiles[index].full_rect.size.height, tiles[index + columnCount].unique_rect.top_left.y);
            EXPECT_EQ(tiles[index].unique_rect.top_left.x + tiles[index].unique_rect.size.width, tiles[index + 1].full_rect.top_left.x);
            EXPECT_EQ(tiles[index].unique_rect.top_left.y + tiles[index].unique_rect.size.height, tiles[index + columnCount].full_rect.top_left.y);
        }
    }
}

TEST_F(TilingTest, ReturnsEvenNumberOfTiles) {
    const tiling::size input_size(3000, 3000);
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 2 * 2);

    CheckStartCoordinates(tiles);
    CheckEndCoordinates(tiles, input_size);

    CheckOverlap(tiles, parameters, 2, 2);
}

TEST_F(TilingTest, ReturnsOddNumberOfTiles) {
    const tiling::size input_size(5000, 5000);
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 3 * 3);

    CheckStartCoordinates(tiles);
    CheckEndCoordinates(tiles, input_size);

    CheckOverlap(tiles, parameters, 3, 3);
}

TEST_F(TilingTest, ReturnsEvenNumberOfTilesForEvenOverlap) {
    const tiling::size input_size(3000, 3000);
    tiling::parameters parameters;
    parameters.overlap_x = 226;
    parameters.overlap_y = 226;
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 2 * 2);

    CheckStartCoordinates(tiles);
    CheckEndCoordinates(tiles, input_size);

    CheckOverlap(tiles, parameters, 2, 2);
}

TEST_F(TilingTest, ReturnsOddNumberOfTilesForEvenOverlap) {
    const tiling::size input_size(5000, 5000);
    tiling::parameters parameters;
    parameters.overlap_x = 226;
    parameters.overlap_y = 226;
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 3 * 3);

    CheckStartCoordinates(tiles);
    CheckEndCoordinates(tiles, input_size);

    CheckOverlap(tiles, parameters, 3, 3);
}

#ifndef NDEBUG
TEST_F(TilingTest, ReservesCorrectAmountOfMemory) {
    for (int width = 1; width < 20; ++width) {
        for (int height = 1; height < 20; ++height) {
            const tiling::size input_size(width, height);
            tiling::parameters parameters;
            parameters.max_tile_height = 8;
            parameters.max_tile_width = 7;
            parameters.overlap_x = 3;
            parameters.overlap_y = 3;
            const auto tiles = tiling::get_tiles(input_size, parameters);
        }
    }
}
#endif // NDEBUG

TEST_F(TilingTest, HandlesNegativeOverlapCorrectly) {
    const tiling::size input_size(35, 25);
    tiling::parameters parameters;
    parameters.max_tile_height = 11;
    parameters.max_tile_width = 7;
    parameters.overlap_x = -3;
    parameters.overlap_y = -3;
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 8);
}

TEST_F(TilingTest, HandlesSpecificEdgeCaseCorrectly) {
    const auto check_output = [](const tiling::size& input_size, const std::vector<tiling::tile>& output) {
        ASSERT_FALSE(output.empty());
        int min_x =  std::numeric_limits<int>::max();
        int min_y =  std::numeric_limits<int>::max();
        int max_x = -std::numeric_limits<int>::max();
        int max_y = -std::numeric_limits<int>::max();

        for (const auto& tile : output) {
            min_x = std::min(min_x, tile.full_rect.top_left.x);
            min_y = std::min(min_y, tile.full_rect.top_left.y);
            max_x = std::max(max_x, tile.full_rect.top_left.x + tile.full_rect.size.width);
            max_y = std::max(max_y, tile.full_rect.top_left.y + tile.full_rect.size.height);
        }

        EXPECT_EQ(min_x, 0);
        EXPECT_EQ(min_y, 0);
        EXPECT_EQ(max_x, input_size.width);
        EXPECT_EQ(max_y, input_size.height);
    };

    tiling::size input_size;
    tiling::parameters parameters;

    for (input_size.width = 659; input_size.width <= 661; ++input_size.width) {
        for (input_size.height = 659; input_size.height <= 661; ++input_size.height) {
            for (parameters.max_tile_width = 355; parameters.max_tile_width <= 357; ++parameters.max_tile_width) {
                for (parameters.max_tile_height = 355; parameters.max_tile_height <= 357; ++parameters.max_tile_height) {
                    for (parameters.overlap_x = 25; parameters.overlap_x <= 27; ++parameters.overlap_x) {
                        for (parameters.overlap_y = 25; parameters.overlap_y <= 27; ++parameters.overlap_y) {
                            const auto tiles = tiling::get_tiles(input_size, parameters);
                            check_output(input_size, tiles);
                        }
                    }
                }
            }
        }
    }
}

TEST_F(TilingTest, IteratorsGenerallyWorkAsExpected) {
    const tiling::size input_size(5000, 5000);
    tiling::parameters parameters;
    const tiling::tiles tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 3 * 3);
    auto i = tiles.begin();
    EXPECT_EQ(i->index.x, 0);
    EXPECT_EQ(i->index.y, 0);
    const auto j = i;
    i++;
    EXPECT_EQ(i->index.x, 1);
    EXPECT_EQ(i->index.y, 0);
    EXPECT_EQ(j->index.x, 0);
    EXPECT_EQ(j->index.y, 0);
}

TEST_F(TilingTest, SupportsOptionalViewportRectangle) {
    const tiling::size input_size(5000, 5000);
    tiling::parameters parameters;
    parameters.viewport_rect = std::make_optional<tiling::rectangle>(
        tiling::point(2500, 4000),
        tiling::size(3000, 500)
    );
    const auto tiles = tiling::get_tiles(input_size, parameters);
    EXPECT_EQ(tiles.size(), 2);
    for (int i = 0; i < tiles.size(); ++i) {
        EXPECT_EQ(tiles[i].index.x, i + 1);
        EXPECT_EQ(tiles[i].index.y, 2);
    }
}

TEST_F(TilingTest, ResultContainsTilesOverlappingOptionalViewportAndNoOtherTiles) {
    const tiling::size input_size(25896, 33943);
    tiling::parameters parameters;
    parameters.max_tile_width  = 497;
    parameters.max_tile_height = 497;
    parameters.overlap_x = 30;
    parameters.overlap_y = 30;
    parameters.limit_to_size = false;

    const auto all_tiles = tiling::get_tiles(input_size, parameters);

    parameters.viewport_rect = std::make_optional<tiling::rectangle>(
        tiling::point(9678, 16907),
        tiling::size(751, 891)
    );

    const auto viewport_tiles = tiling::get_tiles(input_size, parameters);

    const auto is_overlap = [](const tiling::rectangle& lhs, const tiling::rectangle& rhs) {
        if (lhs.top_left.x + lhs.size.width < rhs.top_left.x) {
            return false;
        }
        if (lhs.top_left.y + lhs.size.height < rhs.top_left.y) {
            return false;
        }
        if (rhs.top_left.x + rhs.size.width < lhs.top_left.x) {
            return false;
        }
        if (rhs.top_left.y + rhs.size.height < lhs.top_left.y) {
            return false;
        }
        return true;
    };

    for (const auto& tile : all_tiles) {
        const auto should_be_viewport_tile = is_overlap(tile.full_rect, *parameters.viewport_rect);
        const auto is_viewport_tile = std::find_if(
            viewport_tiles.begin(),
            viewport_tiles.end(),
            [&tile](const tiling::tile& candidate) {
                return candidate.index.x == tile.index.x
                    && candidate.index.y == tile.index.y;
            }
        ) != viewport_tiles.end();
        EXPECT_EQ(should_be_viewport_tile, is_viewport_tile);
    }
}

}  // namespace

int main(int argc, char **argv) {
    return RUN_ALL_TESTS();
}