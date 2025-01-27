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

}  // namespace

int main(int argc, char **argv) {
    return RUN_ALL_TESTS();
}