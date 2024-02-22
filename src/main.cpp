#include "main.hpp"
#include "piece_table.hpp"

#include <ncurses.h>

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
    
    // enable arrow keys to be read
    keypad(stdscr, true);

    // print to screen
    printw("Hello World");

    while (true) {
        // read in a character at a time
        char inputChar = getch();

        if (inputChar == 'q') {
            break;
        }

        // TODO: input handler functions here

        // refreshes the screen
        refresh();
    }

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}