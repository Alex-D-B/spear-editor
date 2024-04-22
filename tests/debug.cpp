#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <sys/param.h>
#include <cassert>

#include "piece_table.hpp"

int main() {
    // Add tests here for easier debugging.
    PieceTable pt("../tests/test2.txt");

    std::cout << pt.toString() << std::endl;

    pt.moveRight();
    pt.moveRight();
    pt.insertChar('a');
    pt.moveLeft();
    pt.moveLeft();
    pt.insertChar('a');

    std::cout << pt.toString() << std::endl;
}