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

void CheckStartCoordinates(const std::vector<tiling::tile>& tiles)
{
    EXPECT_EQ(tiles.front().top_left.x, 0);
    EXPECT_EQ(tiles.front().top_left.y, 0);
}

void CheckEndCoordinates(const std::vector<tiling::tile>& tiles, const tiling::size& size)
{
    const auto& last_tile = tiles.back();
    EXPECT_EQ(last_tile.top_left.x + last_tile.size.width, size.width);
    EXPECT_EQ(last_tile.top_left.y + last_tile.size.height, size.height);
}

void CheckOverlap(const std::vector<tiling::tile>& tiles, const tiling::parameters& parameters, int rowCount, int columnCount)
{
    EXPECT_EQ(tiles.size(), rowCount * columnCount);

    for (int row = 0; row < rowCount - 1; ++row) {
        for (int column = 0; column < columnCount - 1; ++column) {
            const int index = row * columnCount + column;
            EXPECT_EQ(tiles[index].top_left.x + tiles[index].size.width - tiles[index + 1].top_left.x, parameters.overlap_x);
            EXPECT_EQ(tiles[index].top_left.y + tiles[index].size.height - tiles[index + columnCount].top_left.y, parameters.overlap_y);
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

}  // namespace

int main(int argc, char **argv) {
    RUN_ALL_TESTS();
}