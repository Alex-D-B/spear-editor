#include "piece_table.hpp"

#include <fstream>
#include <iostream>

PieceTable::PieceTable() {
    // static_assert(false, "Not implemented");
    exit(1);
}

PieceTable::PieceTable(const char* file_path) {
    // Read the file into the original buffer
    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open()) { std::cerr << "Error opening file\n"; exit(1); }
    else {
        file.seekg(0, std::ios::end); // check if correct
        auto size = file.tellg();
        original = (char*) malloc(size + static_cast<long long>(1)); // +1 for null terminator
        file.seekg(0);
        std::cout << size << std::endl;
        file.read(original, size);
        std::cout << original << std::endl;

        nodes.emplace_back(original, size, false);
    }

    std::cout << original << std::endl;
    added = (char*) malloc(128);
    addedSize = 0;
    addedCapacity = 128;
}
// PieceTable::PieceTable(const PieceTable& other) {
//     static_assert(false, "Not implemented");
// }
// PieceTable::PieceTable(PieceTable&& other) {
//     static_assert(false, "Not implemented");
// }

// PieceTable& PieceTable::operator=(const PieceTable& other) {
//     static_assert(false, "Not implemented");
// }
// PieceTable& PieceTable::operator=(PieceTable&& other) {
//     static_assert(false, "Not implemented");
// }

PieceTable::~PieceTable() {
    free(original);
    free(added);
}

void PieceTable::insertChar(char c) {
    // Make the cursor node point to the end of the added string
    // If the cursor doesn't point to the end of the added string.
    if (getCursorNode().start + cursor.indexInNode != added + addedSize) {
        // split the nodes
        nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                      added + addedSize, 0, true);
        ++cursor.indexOfNode;
    }
    // append to the added string
    reallocAddedIfNeeded();
    Node& cursorNode = getCursorNode();
    added[addedSize] = c;
    ++addedSize;
    ++cursorNode.length;
    ++cursor.indexInNode;
}

void PieceTable::deleteChar() {
    // If the cursor is at position 0, the cursor should move to the previous
    // node. If the cursor is in the middle of a node, the node should be split
    // into two nodes.
    Node& oldCursorNode = getCursorNode();
    // If the cursor isn't at the end of a node.
    if (cursor.indexInNode != oldCursorNode.length) {
        // split the node
        nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                      oldCursorNode.start + cursor.indexInNode,
                      oldCursorNode.length - cursor.indexInNode,
                      oldCursorNode.isAdded);
        getCursorNode().length = cursor.indexInNode;
    }
    // move the cursor to the previous node, and delete the character
    Node& cursorNode = getCursorNode();
    --cursorNode.length;
    --cursor.indexInNode;
    if (cursorNode.length == 0) {
        // remove the node
        nodes.erase(nodes.begin() + cursor.indexOfNode);
        --cursor.indexOfNode;
        cursor.indexInNode = nodes[cursor.indexOfNode].length;
    }
}

void PieceTable::setCursor(size_t index) {
    // TODO: be sure to handle setting the cursor to the end of the file
    size_t nodeIdx = 0;
    while (index > nodes[nodeIdx].length) {
        index -= nodes[nodeIdx].length;
        ++nodeIdx;
    }
    cursor.indexOfNode = nodeIdx;
    cursor.indexInNode = index;
}

void PieceTable::reallocAddedIfNeeded() {
    // append to the added string
    if (addedSize == addedCapacity) {
        addedCapacity *= 2;
        if (addedCapacity == 0) { addedCapacity = 8; }
        char* newAdded = (char*) realloc(added, addedCapacity);

        if (newAdded != added) { // update the pointers for our nodes
            for (Node& node : nodes) {
                node.start = newAdded + (added - node.start);
            }
            added = newAdded;
        }
    }
}
