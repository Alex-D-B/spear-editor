#include <catch2/catch_test_macros.hpp>

#include "piece_table.hpp"

TEST_CASE("See if catch2 works", "[catch2]") {
    REQUIRE(1 == 1);
    PieceTable pt("tests/test.txt");
}