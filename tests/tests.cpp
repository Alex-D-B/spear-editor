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
        ".a");
    }

    SECTION("Insert single character in the middle") {
        pt.setCursor(5);
        pt.insertChar('a');
        REQUIRE(pt.toString() == "This ais a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert single character after a newline") {
        pt.setCursor(31);
        pt.insertChar('a');
        REQUIRE(pt.toString() == "This is a file with test text!\naThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert single character at the end of a line") {
        pt.setCursor(30);
        pt.insertChar('a');
        REQUIRE(pt.toString() == "This is a file with test text!a\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert single character at many locations") {
        pt.setCursor(0);
        pt.insertChar('A');
        pt.setCursor(6);
        pt.insertChar('B');
        pt.setCursor(33);
        pt.insertChar('C');
        pt.setCursor(32);
        pt.insertChar('D');
        pt.setCursor(pt.toString().length());
        pt.insertChar('E');
        REQUIRE(pt.toString() == "AThis Bis a file with test text!D\nCThis is t"
        "he next line. followed by an empty line.\n\nThe line before this is em"
        "pty.E");
    }

    SECTION("Insert multiple characters") {
        pt.insertChar('A');
        pt.insertChar('B');
        pt.insertChar('C');
        pt.setCursor(1);
        pt.insertChar('D');
        REQUIRE(pt.toString() == "ADBCThis is a file with test text!\nThis is th"
        "e next line. followed by an empty line.\n\nThe line before this is emp"
        "ty.");
    }

    SECTION("Insert multiple characters at many locations") {
        pt.insertChar('A');
        pt.insertChar('B');
        pt.setCursor(7);
        pt.insertChar('C');
        pt.insertChar('D');
        pt.setCursor(35);
        pt.insertChar('E');
        pt.insertChar('F');
        pt.setCursor(34);
        pt.insertChar('G');
        pt.insertChar('H');
        pt.setCursor(pt.toString().length());
        pt.insertChar('I');
        pt.insertChar('J');
        REQUIRE(pt.toString() == "ABThis CDis a file with test text!GH\nEFThis"
        " is the next line. followed by an empty line.\n\nThe line before this"
        " is empty.IJ");
    }
}