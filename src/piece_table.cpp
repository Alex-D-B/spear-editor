#include "piece_table.hpp"

#include <assert.h>
#include <iostream>
#include <ncurses.h>

PieceTable::PieceTable() : PieceTable("Untitled") {}

constexpr std::ios::openmode baseFileModes = std::ios::in | std::ios::out | std::ios::ate;

PieceTable::PieceTable(const char* filePath) : cursor{0, 0}, filePath {filePath} {
    // Read the file into the original buffer
    file = std::fstream(filePath, baseFileModes | std::ios::app);
    assert(file.is_open());

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
    file.close();
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
    moveRight();
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

void PieceTable::saveToFile() {
    file.close();
    file.open(filePath, baseFileModes | std::ios::trunc);
    assert(file.is_open());
    file << toString();
}

bool PieceTable::incrementCursor(Cursor& cur) const {
    if (cur.indexInNode == nodes[cur.indexOfNode].length) {
        if (cur.indexOfNode == nodes.size() - 1) {
            return true;
        }
        ++cur.indexOfNode;
        cur.indexInNode = 1;
    } else {
        ++cur.indexInNode;
    }
    return false;
}

bool PieceTable::incrementCursor(Cursor& cur, size_t amount) const {
    // TODO: optimize
    for (size_t i = 0; i < amount; ++i) {
        if (incrementCursor(cur)) {
            return true;
        }
    }
    return false;
}

bool PieceTable::decrementCursor(Cursor& cur) const {
    if (cur.indexInNode == 0) {
        if (cur.indexOfNode == 0) {
            return true;
        }
        --cur.indexOfNode;
        cur.indexInNode = nodes[cur.indexOfNode].length - 1;
    } else {
        --cur.indexInNode;
    }
    return false;
}

bool PieceTable::decrementCursor(Cursor& cur, size_t amount) const {
    // TODO: optimize
    for (size_t i = 0; i < amount; ++i) {
        if (decrementCursor(cur)) {
            return true;
        }
    }
    return false;
}

char PieceTable::getCharAtCursor(Cursor& cur) const {
    if (cur.indexOfNode == nodes.size()
            || (cur.indexOfNode == nodes.size() - 1
                    && cur.indexInNode == nodes[cur.indexOfNode].length)) {
        return '\0';
    } else if (cur.indexInNode == nodes[cur.indexOfNode].length) {
        return nodes[cur.indexOfNode + 1].start[0];
    }
    return nodes[cur.indexOfNode].start[cur.indexInNode];
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
    char c;
    while ((c = getCharAtCursor(tempCursor)) != '\n' && c != '\0') {
        // update temp cursor position
        if (incrementCursor(tempCursor)) {
                --length; // get rid of '\0' at the end
                break;
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
        if (decrementCursor(tempCursor)) {
            break;
        }
    } while (getCharAtCursor(tempCursor) != '\n');

    return length;
}

size_t PieceTable::lineNumber() const {
    size_t number = 1;
    Cursor tempCur = cursor;
    while (true) {
        if (decrementCursor(tempCur)) {
            return number;
        }

        if (nodes[tempCur.indexOfNode].start[tempCur.indexInNode] == '\n') {
            ++number;
        }
    }
}

size_t PieceTable::numLines() const {
    size_t number = 1;
    for (const Node& node : nodes) {
        for (size_t i = 0; i < node.length; ++i) {
            if (node.start[i] == '\n') {
                ++number;
            }
        }
    }
    return number;
}

void PieceTable::moveLeft() {
    // First update internal representation
    if (decrementCursor(cursor)) {
        return;
    }
    // Prefer moving the cursor to the end of the previous node instead of the
    // beginning of the current node.
    if (cursor.indexInNode == 0 && cursor.indexOfNode > 0) {
        --cursor.indexOfNode;
        cursor.indexInNode = getCursorNode().length;
    }

    // Update absolute representation.
    if (getcurx(stdscr) == 0) {
        move(getcury(stdscr) - 1, lineLength() - 1);
    } else {
        move(getcury(stdscr), getcurx(stdscr) - 1);
    }
}

void PieceTable::moveRight() {
    // First update internal representation
    if (incrementCursor(cursor)) {
        return;
    }
    assert(cursor.indexInNode <= getCursorNode().length);
    char prevChar = getCursorNode().start[cursor.indexInNode - 1];

    // Update absolute representation.
    if (prevChar == '\n') {
        move(getcury(stdscr) + 1, 0);
    } else {
        move(getcury(stdscr), getcurx(stdscr) + 1);
    }
}

void PieceTable::moveUp() {
    size_t line = lineNumber();
    if (line == 1) {
        return;
    }

    size_t x = getcurx(stdscr);
    size_t y = getcury(stdscr);

    // Move to one before '\n' on the previous line.
    decrementCursor(cursor, x + 1);
    if (x < lineLength()) {
        // New position will land somewhere in the middle of the line above.
        decrementCursor(cursor, lineLength() - x - 1);
        move(y - 1, x);
    } else {
        // New position will land at the end of the line above.
        move(y - 1, lineLength() - 1);
    }
}

void PieceTable::moveDown() {
    size_t line = lineNumber();
    if (line == numLines()) {
        return;
    }

    size_t x = getcurx(stdscr);
    size_t y = getcury(stdscr);

    // Move to the beginning of the next line.
    incrementCursor(cursor, lineLength() - x);
    if (x < lineLength()) {
        // New position will land somewhere in the middle of the line below.
        incrementCursor(cursor, x);
        move(y + 1, x);
    } else {
        // New position will land at the beginning of the line below.
        move(y + 1, 0);
    }
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