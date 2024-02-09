#pragma once

#include <vector>

class PieceTable final {
public:
    PieceTable();
    PieceTable(const char* file_path);
    PieceTable(const PieceTable& other);
    PieceTable(PieceTable&& other);

    PieceTable& operator=(const PieceTable& other);
    PieceTable& operator=(PieceTable&& other);

    ~PieceTable();

    void insertChar(char c);
    void deleteChar();
    void setCursor(size_t index);

private:
    struct Node {
        char* start;
        size_t length; // Number of characters in the node
    };

    // Rcursor position. This might not be the same
    // as the user's cursor position. This will stay in the same position until
    // the user edits at a different position.
    epresentation of the current struct Cursor {
        Node* node;
        size_t index;
    };

    // Original file data (heap allocated)
    char* original;
    // Added data (heap allocated)
    char* added;

    // The current cursor position, used for insert and delete operations
    Cursor cursor;

    std::vector<Node> nodes;

};