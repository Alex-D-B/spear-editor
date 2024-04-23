#include "main.hpp"
#include "piece_table.hpp"

#include <ncurses.h>

constexpr int CTRL_C = 0x3;
constexpr int CTRL_S = 0x13;

constexpr int unsavedColor = 1;
constexpr int savedColor = 2;

int main(int argc, char** argv) {
    // init screen and sets up screen
    initscr();

    if (has_colors() == FALSE) {
        endwin();
		printf("Your terminal does not support color :(\n");
		exit(1);
	}
    start_color();
    init_color(COLOR_YELLOW, 500, 500, 500);
    init_pair(unsavedColor, COLOR_RED, COLOR_YELLOW);
    init_pair(savedColor, COLOR_GREEN, COLOR_YELLOW);

    PieceTable pt = argc == 1 ? PieceTable() : PieceTable(argv[1]);

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

    bool unsavedChanges = false;

    while (true) {
        // Store current cursor position.
        int x = getcurx(stdscr);
        int y = getcury(stdscr);
        // mvprintw(7, 0, "%s", pt.toString().c_str());
        int xMax, yMax; 
        getmaxyx(stdscr, yMax, xMax);
        chtype colorPair = COLOR_PAIR(unsavedChanges ? unsavedColor : savedColor);
        attron(colorPair);
        mvprintw(yMax - 1, 0, "%s", unsavedChanges ? " Unsaved changes to " : " Saved ");
        printw("%s", pt.getFilePath());
        printw(" ");
        attroff(colorPair);
        clrtoeol();
        // Restore old cursor position.
        move(y, x);

        // refreshes the screen
        refresh();

        // read in a character at a time
        int inputChar = wgetch(stdscr);

        if (inputChar == CTRL_C) {
            break;
        }

        unsavedChanges = true;
        switch (inputChar) {
            case KEY_LEFT:
                pt.moveLeft();
                break;
            case KEY_RIGHT:
                pt.moveRight();
                break;
            case KEY_UP:
                pt.moveUp();
                break;
            case KEY_DOWN:
                pt.moveDown();
                break;
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                pt.deleteChar();
                break;
            case CTRL_S:
                pt.saveToFile();
                unsavedChanges = false;
        }

        if (isalnum(inputChar) || ispunct(inputChar) || isspace(inputChar)) {
            pt.insertChar(inputChar);
        }
    }

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}