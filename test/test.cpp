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

TEST_F(TilingTest, ReturnsEvenNumberOfTiles) {
    const auto tiles = tiling::get_tiles(tiling::size(3000, 3000), tiling::parameters());
    EXPECT_EQ(tiles.size(), 2 * 2);
}

TEST_F(TilingTest, ReturnsOddNumberOfTiles) {
    const auto tiles = tiling::get_tiles(tiling::size(5000, 5000), tiling::parameters());
    EXPECT_EQ(tiles.size(), 3 * 3);
}

}  // namespace

int main(int argc, char **argv) {
    RUN_ALL_TESTS();
}