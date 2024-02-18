#include "piece_table.hpp"

#include <fstream>
#include <iostream>

PieceTable::PieceTable() {
    // static_assert(false, "Not implemented");
    exit(1);
}

PieceTable::PieceTable(const char* file_path) : cursor{0, 0} {
    // Read the file into the original buffer
    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open()) { std::cerr << "Error opening file\n"; exit(1); }

    file.seekg(0, std::ios::end); // check if correct
    auto size = file.tellg();
    original = (char*) malloc(size + static_cast<long long>(1)); // +1 for null terminator
    file.seekg(0);
    // std::cout << size << std::endl;
    file.read(original, size);
    // std::cout << original << std::endl;

    // std::cout << original << std::endl;
    added = (char*) malloc(128);
    addedSize = 0;
    addedCapacity = 128;

    if (size == 0) {
        nodes.emplace_back(added, 0, true);
    } else {
        nodes.emplace_back(original, size, false);
    }
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
        if (cursor.indexInNode == 0) {
            // If the cursor is at the beginning of a node...
            // Insert a new node in front of the current node.
            nodes.emplace(nodes.begin() + cursor.indexOfNode,
                          added + addedSize, 0, true);
        } else if (cursor.indexInNode == getCursorNode().length) {
            // If the cursor is at the end of a node...
            // Insert a new node after the current node.
            nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                          added + addedSize, 0, true);
            ++cursor.indexOfNode;
            cursor.indexInNode = 0;
        } else { // The cursor is in the middle of a node.
            // Split the node into two nodes.
            nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                          getCursorNode().start + cursor.indexInNode,
                          getCursorNode().length - cursor.indexInNode,
                          getCursorNode().isAdded);
            // Update the original node.
            getCursorNode().length = cursor.indexInNode;

            // Insert new node in between the split nodes for cursor insertion.
            nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                          added + addedSize, 0, true);
            ++cursor.indexOfNode;
            cursor.indexInNode = 0;
        }
    }

    // append to the added string
    reallocAddedIfNeeded();
    added[addedSize] = c;
    ++addedSize;
    ++getCursorNode().length;
    ++cursor.indexInNode;
}

void PieceTable::deleteChar() {
    // If the cursor is at position 0, the cursor should move to the previous
    // node. If the cursor is in the middle of a node, the node should be split
    // into two nodes.
    if (cursor.indexInNode == 0) {
        // If at the beginning of the file, don't do anything.
        if (cursor.indexOfNode == 0) { 
            return;
        }
        // Move the cursor to the previous node.
        --cursor.indexOfNode;
        cursor.indexInNode = getCursorNode().length;
    } else if (cursor.indexInNode != getCursorNode().length) {
        // If the cursor isn't at the end of a node...
        // split the node
        nodes.emplace(nodes.begin() + cursor.indexOfNode + 1,
                      getCursorNode().start + cursor.indexInNode,
                      getCursorNode().length - cursor.indexInNode,
                      getCursorNode().isAdded);
        getCursorNode().length = cursor.indexInNode;
    }
    Node& cursorNode = getCursorNode(); // might have moved after emplace
    --cursorNode.length;
    --cursor.indexInNode;
    if (cursorNode.length == 0) {
        // remove the node
        nodes.erase(nodes.begin() + cursor.indexOfNode);
        // Add sentinel node if the file is now empty.
        if (nodes.size() == 0) {
            nodes.emplace_back(added + addedSize, 0, true);
        } else if (cursor.indexOfNode != 0) {
            --cursor.indexOfNode;
            cursor.indexInNode = nodes[cursor.indexOfNode].length;
        }
    }

}

void PieceTable::setCursor(size_t index) {
    size_t nodeIdx = 0;
    while (index > nodes[nodeIdx].length) {
        index -= nodes[nodeIdx].length;
        ++nodeIdx;
    }
    cursor.indexOfNode = nodeIdx;
    cursor.indexInNode = index;
}

std::string PieceTable::toString() const {
    std::string result;
    for (const Node& node : nodes) {
        result.append(node.start, node.length);
    }
    return result;   
}

void PieceTable::reallocAddedIfNeeded() {
    // append to the added string
    if (addedSize == addedCapacity) {
        addedCapacity *= 2;
        if (addedCapacity == 0) { addedCapacity = 8; }
        char* newAdded = (char*) realloc(added, addedCapacity);

        if (newAdded != added) { // update the pointers for our nodes
            for (Node& node : nodes) {
                if (node.isAdded) {
                    node.start = newAdded + (node.start - added);
                }
            }
            added = newAdded;
        }
    }
}
