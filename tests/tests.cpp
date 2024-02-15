#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "piece_table.hpp"

TEST_CASE("PieceTable basic constructor from file", "[catch2]") {
    REQUIRE(1 == 1);
    PieceTable pt("../tests/test1.txt");
}

TEST_CASE("PieceTable toString test1", "[catch2]") {
    PieceTable pt("../tests/test1.txt");

    REQUIRE(pt.toString() == "This is a file with test text!\nThis is the next "
    "line. followed by an empty line.\n\nThe line before this is empty.");
}

TEST_CASE("PieceTable insertChar", "[catch2]") {
    PieceTable pt("../tests/test1.txt");

    SECTION("Insert single character at the beginning") {
        pt.insertChar('a');
        REQUIRE(pt.toString() == "aThis is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert single character at the end") {
        pt.setCursor(pt.toString().length());
        pt.insertChar('a');
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        "a");
    }
}