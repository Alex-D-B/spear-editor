#include "main.hpp"
#include "piece_table.hpp"

#include <ncurses.h>

constexpr int CTRL_C = 0x3;

int main(int argc, char** argv) {
    // init screen and sets up screen
    initscr();

    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        exit(1);
    }

    PieceTable pt = PieceTable(argv[1]);

    // forward input to program, including things like Ctrl+C
    raw();

    // do not automatically echo input so we can validate it first
    noecho();
    
    // enable arrow keys to be read
    keypad(stdscr, true);

    // print to screen
    printw("%s", pt.toString().c_str());

    // start the cursor at the beginning of the file
    move(0, 0);

    while (true) {
        // read in a character at a time
        int inputChar = wgetch(stdscr);

        if (inputChar == CTRL_C) {
            break;
        }

        switch (inputChar) {
            case KEY_LEFT:
                pt.moveLeft();
                break;
            case KEY_RIGHT:
                pt.moveRight();
                break;
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                pt.deleteChar();
                break;
        }

        if (isalnum(inputChar) || ispunct(inputChar) || isspace(inputChar)) {
            pt.insertChar(inputChar);
        }

        int x = getcurx(stdscr);
        int y = getcury(stdscr);
        mvprintw(5, 0, "%s", pt.toString().c_str());
        move(y, x);

        // refreshes the screen
        refresh();
    }

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}