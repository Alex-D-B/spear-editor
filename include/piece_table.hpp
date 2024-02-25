#pragma once

#include <vector>
#include <string>

class PieceTable final {
public:
    PieceTable();

    /**
     * Create a new PieceTable from the given file. Cursor starts at the
     * beginning.
     * 
     * @param file_path The path to the file to read.
     */
    PieceTable(const char* file_path);

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
    
    // Move the cursor to the left. Wraps to the previous line if necessary.
    void moveLeft();
    // Move the cursor to the right. Wraps to the next line if necessary.
    void moveRight();
    // Move the cursor up.
    void moveUp();
    // Move the cursor down.
    void moveDown();

    #ifdef DEBUG_MODE
    size_t lineLengthPUB() const {
        return lineLength();
    }
    #endif

private:
    struct Node final {
        char* start; // Points to text in either the original or added string
        size_t length; // Number of characters in the node
        bool isAdded; // Whether the node is from the added string.

        Node(char* start, size_t length, bool isAdded)
            : start(start), length(length), isAdded(isAdded) {}
    };

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


    // Original file data (heap allocated)
    char* original;
    // Added data (heap allocated)
    char* added;
    size_t addedSize;
    size_t addedCapacity;

    // The current cursor position, used for insert and delete operations
    Cursor cursor;
    std::vector<Node> nodes;

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
};