#pragma once

#include <fstream>
#include <string>
#include <vector>

class PieceTable final {
public:
    PieceTable();

    /**
     * Create a new PieceTable from the given file. Cursor starts at the
     * beginning.
     * 
     * @param filePath The path to the file to read.
     */
    PieceTable(const char* filePath);

    ~PieceTable();

    /**
     * Insert a character at the current cursor position. If the cursor is in the 
     * middle of a node, the node should be split into two nodes.
     * 
     * @param c The character to insert.
     */
    void insertChar(char c);
    void deleteChar();

    /**
     * Set the cursor to the given index.
     * 
     * @param index The character index of the file. This includes '\n' as a
     *              valid character.
     */
    void setCursor(size_t index);

    // Return the entire file as a string.
    std::string toString() const;

    // Saves the current contents to the opened file
    void saveToFile();
    // Get the path of the file.
    std::string_view getFilePath() const { return filePath; }
    
    // Move the cursor to the left. Wraps to the previous line if necessary.
    void moveLeft();
    // Move the cursor to the right. Wraps to the next line if necessary.
    void moveRight();
    // Move the cursor up.
    void moveUp();
    // Move the cursor down.
    void moveDown();

#ifndef DEBUG_MODE
private:
#endif

    struct Node final {
        char* start; // Points to text in either the original or added string
        size_t length; // Number of characters in the node
        bool isAdded; // Whether the node is from the added string.

        Node(char* start, size_t length, bool isAdded)
            : start(start), length(length), isAdded(isAdded) {}
    };

#ifdef DEBUG_MODE
    size_t lineLengthPUB() const {
        return lineLength();
    }
    struct PosPair {
        int indexInNode;
        int indexOfNode;
    };
    PosPair getCursorPos() const {
        return {static_cast<int>(cursor.indexInNode), static_cast<int>(cursor.indexOfNode)};
    }

    // Get the global index of the cursor in the current file string.
    size_t getGlobalCharIndex() {
        size_t index = 0;
        for (size_t i = 0; i < cursor.indexOfNode; ++i) {
            index += nodes[i].length;
        }
        index += cursor.indexInNode;
        return index;
    }

    std::vector<Node>& getNodes() {
        return nodes;
    }

private:
#endif

    // Cursor position. This might not be the same
    // as the user's cursor position. This will stay in the same position until
    // the user edits at a different position.
    struct Cursor final {
        // Index of the current cursor node in the list of nodes.
        size_t indexOfNode;
        // Index of the current cursor position in the current node.
        size_t indexInNode;

        bool operator!=(const Cursor& other) const {
            return indexOfNode != other.indexOfNode
                || indexInNode != other.indexInNode;
        }
    };
    // Move the given cursor to the right. Returns true if at the end.
    bool incrementCursor(Cursor& cur) const;
    // Move the given cursor to the right by the given amount. Returns true if
    // it reaches the end.
    bool incrementCursor(Cursor& cur, size_t amount) const;
    // Move the given cursor to the left. Returns true if at the beginning.
    bool decrementCursor(Cursor& cur) const;
    // Move the given cursor to the left by the given amount. Returns true if it
    // reaches the beginning.
    bool decrementCursor(Cursor& cur, size_t amount) const;
    // Get the character at the given cursor position. '\0' if at the end of the
    // file. Wraps to the first character of the next node if necessary.
    char getCharAtCursor(Cursor& cur) const;

    // Original file data (heap allocated)
    char* original;
    // Added data (heap allocated)
    char* added;
    size_t addedSize;
    size_t addedCapacity;

    // The current cursor position, used for insert and delete operations
    Cursor cursor;
    std::vector<Node> nodes;

    // File we are currently editing
    std::fstream file;
    // Path to the file we are currently editing
    std::string filePath;

    // Increase the capacity of the added string, and fix all the node pointers,
    // if addedSize == addedCapacity.
    void reallocAddedIfNeeded();

    // Get the node that the cursor is currently in.
    inline Node& getCursorNode() {
        return nodes[cursor.indexOfNode];
    }

    inline const Node& getCursorNode() const {
        return nodes[cursor.indexOfNode];
    }

    // Get the length of the current line. '\n' is included in the length.
    size_t lineLength() const;
    // Get the line number of the current line. (starting at 1)
    size_t lineNumber() const;
    // Get the total number of lines in the file.
    size_t numLines() const;
};