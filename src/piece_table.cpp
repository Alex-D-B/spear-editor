#include "piece_table.hpp"

#include <fstream>
#include <iostream>

PieceTable::PieceTable() {
    static_assert(false, "Not implemented");
}

/**
 * Create a new PieceTable from the given file.
 * 
 * @param file_path The path to the file to read.
 */
PieceTable::PieceTable(const char* file_path) {
    // Read the file into the original buffer
    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open()) { std::cerr << "Error opening file\n"; }
    else {
        file.seekg(0, std::ios::end); // check if correct
        auto size = file.tellg();
        original = (char*) malloc(size + static_cast<long long>(1)); // +1 for null terminator
        file.seekg(0);
        std::cout << size << std::endl;
        file.read(original, size);
        std::cout << original << std::endl;
    }

    std::cout << original << std::endl;
    added = (char*) malloc(128);
}
PieceTable::PieceTable(const PieceTable& other) {
    static_assert(false, "Not implemented");
}
PieceTable::PieceTable(PieceTable&& other) {
    static_assert(false, "Not implemented");
}

PieceTable& PieceTable::operator=(const PieceTable& other) {
    static_assert(false, "Not implemented");
}
PieceTable& PieceTable::operator=(PieceTable&& other) {
    static_assert(false, "Not implemented");
}

PieceTable::~PieceTable() {
    static_assert(false, "Not implemented");
}

/**
 * Insert a character at the current cursor position. If the cursor is in the 
 * middle of a node, the node should be split into two nodes.
 * 
 * @param c The character to insert.
 */
void PieceTable::insertChar(char c) {
    static_assert(false, "Not implemented");
}

/**
 * Delete the character just before the current cursor position.
 */
void PieceTable::deleteChar() {
    // If the cursor is at position 0, the cursor should move to the previous
    // node. If the cursor is in the middle of a node, the node should be split
    // into two nodes.
    static_assert(false, "Not implemented");
}

/**
 * Set the cursor to the given index.
 * 
 * @param index The character index of the file. This includes '\n' as a valid
 *              character.
 */
void PieceTable::setCursor(size_t index) {
    // TODO: be sure to handle setting the cursor to the end of the file
    static_assert(false, "Not implemented");
}

// // Original file data (heap allocated)
// char* original;
// // Added data (heap allocated)
// char* added;

// // The current cursor position, used for insert and delete operations
// Cursor cursor;

// std::vector<Node> nodes;
