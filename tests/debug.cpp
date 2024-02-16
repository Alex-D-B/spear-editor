#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <sys/param.h>

#include "sys_const.hpp" // File to include PROJECT_ROOT - system dependent.
#include "piece_table.hpp"

int main() {
    // Add tests here for easier debugging.
    PieceTable pt(PROJECT_ROOT "tests/test1.txt");

    pt.setCursor(pt.toString().length());
    pt.insertChar('a');
    std::cout << pt.toString() << std::endl;
}