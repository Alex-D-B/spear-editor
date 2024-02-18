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

    SECTION("Insert mass insert") {
        pt.setCursor(pt.toString().length());
        for (int i = 0; i < 1024 - 112; ++i) {
            pt.insertChar('A');
        }
        std::string expected = "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is "
        "empty.";
        for (int i = 0; i < 1024 - 112; ++i) {
            expected += 'A';
        }
        std::string actual = pt.toString();
        REQUIRE(actual.length() == 1024);
        REQUIRE(actual == expected);
    }
}

TEST_CASE("PieceTable deleteChar", "[catch2]") {
    PieceTable pt("../tests/test1.txt");

    SECTION("Delete before all characters") {
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete single character at the beginning") {
        pt.setCursor(1);
        pt.deleteChar();
        REQUIRE(pt.toString() == "his is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete single character at the end") {
        pt.setCursor(pt.toString().length());
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        );
    }

    SECTION("Delete single character in the middle") {
        pt.setCursor(5);
        pt.deleteChar();
        REQUIRE(pt.toString() == "Thisis a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete single character after a newline") {
        pt.setCursor(31);
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!This is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete single character at the end of a line") {
        pt.setCursor(30);
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete single character at many locations") {
        pt.setCursor(0);
        pt.deleteChar();
        pt.setCursor(1);
        pt.deleteChar();
        pt.setCursor(6);
        pt.deleteChar();
        pt.setCursor(33);
        pt.deleteChar();
        pt.setCursor(32);
        pt.deleteChar();
        pt.setCursor(30);
        pt.deleteChar();
        pt.setCursor(pt.toString().length());
        pt.deleteChar();
        REQUIRE(pt.toString() == "his i a file with test text!\nh is the next "
        "line. followed by an empty line.\n\nThe line before this is empty");
    }

    SECTION("Delete multiple characters") {
        pt.setCursor(4);
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        REQUIRE(pt.toString() == "T is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Delete multiple characters at many locations") {
        pt.setCursor(2);
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        REQUIRE(pt.toString() == "is is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(2);
        pt.deleteChar();
        REQUIRE(pt.toString() == "i is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(29);
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        REQUIRE(pt.toString() == "i is a file with test texthis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(pt.toString().length());
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        REQUIRE(pt.toString() == "i is a file with test texthis is the "
        "next line. followed by an empty line.\n\nThe line before this is emp");
    }

    SECTION("Delete entire file") {
        pt.setCursor(pt.toString().length());
        while (pt.toString().length() > 0) {
            pt.deleteChar();
        }
        REQUIRE(pt.toString() == "");
    }
}

TEST_CASE("PieceTable insertChar and deleteChar", "[catch2]") {
    PieceTable pt("../tests/test1.txt");

    SECTION("Insert and delete single character at the beginning") {
        pt.insertChar('a');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete single character at the end") {
        pt.setCursor(pt.toString().length());
        pt.insertChar('a');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete single character in the middle") {
        pt.setCursor(5);
        pt.insertChar('a');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete single character after a newline") {
        pt.setCursor(31);
        pt.insertChar('a');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete single character at the end of a line") {
        pt.setCursor(30);
        pt.insertChar('a');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete single character at many locations") {
        pt.setCursor(0);
        pt.insertChar('A');
        pt.deleteChar();
        pt.setCursor(6);
        pt.insertChar('B');
        pt.deleteChar();
        pt.setCursor(33);
        pt.insertChar('C');
        pt.deleteChar();
        pt.setCursor(32);
        pt.insertChar('D');
        pt.deleteChar();
        pt.setCursor(pt.toString().length());
        pt.insertChar('E');
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete multiple characters") {
        pt.insertChar('A');
        pt.insertChar('B');
        pt.insertChar('C');
        pt.insertChar('D');
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        pt.setCursor(1);
        pt.insertChar('E');
        pt.insertChar('F');
        pt.setCursor(2);
        pt.insertChar('G');
        pt.insertChar('H');
        pt.setCursor(2);
        pt.deleteChar();
        pt.setCursor(4);
        pt.deleteChar();
        pt.deleteChar();
        pt.deleteChar();
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete multiple characters at many locations") {
        pt.insertChar('A');
        pt.insertChar('B');
        pt.deleteChar();
        pt.insertChar('C');
        REQUIRE(pt.toString() == "ACThis is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(0);
        pt.insertChar('D');
        pt.setCursor(4);
        pt.deleteChar();
        REQUIRE(pt.toString() == "DAChis is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(7);
        for (int i = 0; i < 7; ++i) pt.deleteChar();
        REQUIRE(pt.toString() == "is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
    }

    SECTION("Insert and delete mass number of chars") {
        for (int i = 0; i < 1024 - 112; ++i) {
            pt.insertChar('A');
        }
        for (int i = 0; i < 1024 - 112; ++i) {
            pt.deleteChar();
        }
        REQUIRE(pt.toString() == "This is a file with test text!\nThis is the "
        "next line. followed by an empty line.\n\nThe line before this is empty"
        ".");
        pt.setCursor(pt.toString().length());
        while (pt.toString().length() > 0) {
            pt.deleteChar();
        }
        REQUIRE(pt.toString() == "");
        for (int i = 0; i < 1024; ++i) {
            pt.insertChar('A');
        }
        pt.setCursor(100);
        for (int i = 0; i < 2048; ++i) {
            pt.insertChar('B');
        }
        pt.setCursor(pt.toString().length());
        while (pt.toString().length() > 0) {
            pt.deleteChar();
        }
        REQUIRE(pt.toString() == "");
        for (int i = 0; i < 1024; ++i) {
            pt.insertChar('A');
        }
        for (int i = 256; i > 0; --i) {
            pt.setCursor(i * 4);
            pt.deleteChar();
        }
        for (int i = 0; i < 256; ++i) {
            pt.setCursor(i * 4);
            pt.insertChar('B');
        }
        std::string result = pt.toString();
        REQUIRE(result.length() == 1024);
        bool success_flag = true;
        for (int i = 0; i < 1024; ++i) {
            if (i % 4 == 0 && result[i] != 'B') {
                success_flag = false;
                break;
            } else if (i % 4 != 0 && result[i] != 'A') {
                success_flag = false;
                break;
            }
        }
        REQUIRE(success_flag);
        pt.setCursor(pt.toString().length());
        while (pt.toString().length() > 0) {
            pt.deleteChar();
        }
        REQUIRE(pt.toString() == "");
        /*
        */
    }
}
