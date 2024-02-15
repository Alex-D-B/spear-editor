#pragma once

#include <vector>
#include <string>

class PieceTable final {
public:
    PieceTable();


    /**
     * Create a new PieceTable from the given file.
     * 
     * @param file_path The path to the file to read.
     */
    PieceTable(const char* file_path);
    // PieceTable(const PieceTable& other);
    // PieceTable(PieceTable&& other);

    // PieceTable& operator=(const PieceTable& other);
    // PieceTable& operator=(PieceTable&& other);

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

    std::string toString() const;

private:
    struct Node final {
        char* start; // Points to either the original or added string
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
};