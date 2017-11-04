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
    EXPECT_EQ(tiles[0].full_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].full_rect.size.width, 500);
    EXPECT_EQ(tiles[0].full_rect.size.height, 500);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.width, 500);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.height, 500);
}

TEST_F(TilingTest, HandlesMaxTileSizeImageCorrectly) {
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(tiling::size(parameters.max_tile_width, parameters.max_tile_height), parameters);
    EXPECT_EQ(tiles.size(), 1);
    EXPECT_EQ(tiles[0].full_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].full_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].full_rect.size.width, parameters.max_tile_width);
    EXPECT_EQ(tiles[0].full_rect.size.height, parameters.max_tile_height);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.top_left.y, 0);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.width, parameters.max_tile_width);
    EXPECT_EQ(tiles[0].non_overlapping_rect.size.height, parameters.max_tile_height);
}

void CheckStartCoordinates(const std::vector<tiling::tile>& tiles)
{
    EXPECT_EQ(tiles.front().full_rect.top_left.x, 0);
    EXPECT_EQ(tiles.front().full_rect.top_left.y, 0);
    EXPECT_EQ(tiles.front().non_overlapping_rect.top_left.x, 0);
    EXPECT_EQ(tiles.front().non_overlapping_rect.top_left.y, 0);
}

void CheckEndCoordinates(const std::vector<tiling::tile>& tiles, const tiling::size& size)
{
    const auto& last_tile = tiles.back();
    EXPECT_EQ(last_tile.full_rect.top_left.x + last_tile.full_rect.size.width, size.width);
    EXPECT_EQ(last_tile.full_rect.top_left.y + last_tile.full_rect.size.height, size.height);
    EXPECT_EQ(last_tile.non_overlapping_rect.top_left.x + last_tile.non_overlapping_rect.size.width, size.width);
    EXPECT_EQ(last_tile.non_overlapping_rect.top_left.y + last_tile.non_overlapping_rect.size.height, size.height);
}

void CheckOverlap(const std::vector<tiling::tile>& tiles, const tiling::parameters& parameters, int rowCount, int columnCount)
{
    EXPECT_EQ(tiles.size(), rowCount * columnCount);

    for (int row = 0; row < rowCount - 1; ++row) {
        for (int column = 0; column < columnCount - 1; ++column) {
            const int index = row * columnCount + column;
            EXPECT_EQ(tiles[index].full_rect.top_left.x + tiles[index].full_rect.size.width - tiles[index + 1].full_rect.top_left.x, parameters.overlap_x);
            EXPECT_EQ(tiles[index].full_rect.top_left.y + tiles[index].full_rect.size.height - tiles[index + columnCount].full_rect.top_left.y, parameters.overlap_y);
            EXPECT_EQ(tiles[index].non_overlapping_rect.top_left.x + tiles[index].non_overlapping_rect.size.width, tiles[index + 1].non_overlapping_rect.top_left.x);
            EXPECT_EQ(tiles[index].non_overlapping_rect.top_left.y + tiles[index].non_overlapping_rect.size.height, tiles[index + columnCount].non_overlapping_rect.top_left.y);
        }
    }
}

TEST_F(TilingTest, ReturnsEvenNumberOfTiles) {
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(tiling::size(3000, 3000), parameters);
    EXPECT_EQ(tiles.size(), 2 * 2);

    CheckOverlap(tiles, parameters, 2, 2);
}

TEST_F(TilingTest, ReturnsOddNumberOfTiles) {
    const tiling::parameters parameters;
    const auto tiles = tiling::get_tiles(tiling::size(5000, 5000), parameters);
    EXPECT_EQ(tiles.size(), 3 * 3);

    CheckOverlap(tiles, parameters, 3, 3);
}

TEST_F(TilingTest, ReturnsEvenNumberOfTilesForEvenOverlap) {
    tiling::parameters parameters;
    parameters.overlap_x = 226;
    parameters.overlap_y = 226;
    const auto tiles = tiling::get_tiles(tiling::size(3000, 3000), parameters);
    EXPECT_EQ(tiles.size(), 2 * 2);

    CheckOverlap(tiles, parameters, 2, 2);
}

TEST_F(TilingTest, ReturnsOddNumberOfTilesForEvenOverlap) {
    tiling::parameters parameters;
    parameters.overlap_x = 226;
    parameters.overlap_y = 226;
    const auto tiles = tiling::get_tiles(tiling::size(5000, 5000), parameters);
    EXPECT_EQ(tiles.size(), 3 * 3);

    CheckOverlap(tiles, parameters, 3, 3);
}

}  // namespace

int main(int argc, char **argv) {
    return RUN_ALL_TESTS();
}