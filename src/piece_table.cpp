#include "piece_table.hpp"

#include <fstream>
#include <assert.h>
#include <iostream>
#include <ncurses.h>

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
    // +1 for null terminator
    original = (char*) malloc(static_cast<size_t>(size) + 1);
    file.seekg(0);
    file.read(original, size);

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

    // Update the terminal display
    insch(c);
    // Move cursor to the right
    moveRight(true);
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

    // Update the terminal display
    move(getcury(stdscr), getcurx(stdscr) - 1);
    delch();
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

size_t PieceTable::lineLength() const {
    size_t length = 0;
    Cursor tempCursor = cursor;
    while (getCursorNode().start[tempCursor.indexInNode] != '\n') {
        // update temp cursor position
        if (tempCursor.indexInNode == getCursorNode().length) {
            if (tempCursor.indexOfNode == nodes.size() - 1) {
                --length; // get rid of '\0' at the end
                break;
            }
            ++tempCursor.indexOfNode;
            tempCursor.indexInNode = 0;
        } else {
            ++tempCursor.indexInNode;
        }
        ++length;
    }
    ++length; // count new line at the end
    
    tempCursor = cursor;
    do {
        if (tempCursor != cursor) {
            ++length;
        }
        // update temp cursor position
        if (tempCursor.indexInNode == 0) {
            if (tempCursor.indexOfNode == 0) {
                break;
            }
            --tempCursor.indexOfNode;
            tempCursor.indexInNode = nodes[tempCursor.indexOfNode].length - 1;
        } else {
            --tempCursor.indexInNode;
        }
    } while (getCursorNode().start[tempCursor.indexInNode] != '\n');

    return length;
}

void PieceTable::moveLeft() {
    // First update internal representation
    if (cursor.indexInNode == 0) {
        if (cursor.indexOfNode == 0) {
            // int x = getcurx(stdscr);
            // int y = getcury(stdscr);
            // mvprintw(3, 0, "%s", std::to_string(cursor.indexOfNode).c_str());
            // mvprintw(4, 0, "%s", std::to_string(cursor.indexInNode).c_str());
            // mvprintw(5, 0, "%s", std::to_string(nodes.size()).c_str());
            // move(y, x);
            return;
        }
        --cursor.indexOfNode;
        cursor.indexInNode = getCursorNode().length;
    }
    --cursor.indexInNode;

    // Update absolute representation.
    int x = getcurx(stdscr);
    if (x == 0) {
        move(getcury(stdscr) - 1, lineLength());
    } else {
        move(getcury(stdscr), getcurx(stdscr) - 1);
    }
    // x = getcurx(stdscr);
    // int y = getcury(stdscr);
    // mvprintw(3, 0, "%s", std::to_string(cursor.indexOfNode).c_str());
    // mvprintw(4, 0, "%s", std::to_string(cursor.indexInNode).c_str());
    // mvprintw(5, 0, "%s", std::to_string(nodes.size()).c_str());
    // move(y, x);
}

void PieceTable::moveRight(bool verbose) {
    // First update internal representation
    if (cursor.indexInNode == getCursorNode().length) {
        if (cursor.indexOfNode == nodes.size() - 1) {
            // int x = getcurx(stdscr);
            // int y = getcury(stdscr);
            // mvprintw(3, 0, "%s", std::to_string(cursor.indexOfNode).c_str());
            // mvprintw(4, 0, "%s", std::to_string(cursor.indexInNode).c_str());
            // mvprintw(5, 0, "%s", std::to_string(nodes.size()).c_str());
            // move(y, x);
            // std::cout << "returned here\n";
            return;
        }
        ++cursor.indexOfNode;
        cursor.indexInNode = 0;
    }
    // if (cursor.indexOfNode == nodes.size() - 1 && cursor.indexInNode == nodes.back().length - 1) {
    //     std::cout << "here" << std::endl;
    //     return;
    // }

    assert(cursor.indexInNode < getCursorNode().length);
    char prevChar = getCursorNode().start[cursor.indexInNode];
    ++cursor.indexInNode;

    // Update absolute representation.
    if (prevChar == '\n') {
        move(getcury(stdscr) + 1, 0);
    } else {
        move(getcury(stdscr), getcurx(stdscr) + 1);
    }

    // int x = getcurx(stdscr);
    // int y = getcury(stdscr);
    // mvprintw(3, 0, "%s", std::to_string(cursor.indexOfNode).c_str());
    // mvprintw(4, 0, "%s", std::to_string(cursor.indexInNode).c_str());
    // mvprintw(5, 0, "%s", std::to_string(nodes.size()).c_str());
    // move(y, x);
}


// #ifdef DEBUG_MODE
// size_t PieceTable::getGlobalCharIndex() {
//     size_t index = 0;
//     for (size_t i = 0; i < cursor.indexOfNode; ++i) {
//         index += nodes[i].length;
//     }
//     index += cursor.indexInNode;
//     return index;
// }
// #endif