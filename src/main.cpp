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
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    WINDOW* pad = newpad(maxY, maxX);

    PieceTable pt = argc == 1 ? PieceTable(pad) : PieceTable(pad, argv[1]);

    // forward input to program, including things like Ctrl+C
    raw();

    // do not automatically echo input so we can validate it first
    noecho();
    
    // enable arrow keys to be read
    keypad(pad, true);

    // print to screen
    wprintw(pad, "%s", pt.toString().c_str());

    // start the cursor at the beginning of the file
    wmove(pad, 0, 0);

    bool unsavedChanges = false;

    while (true) {
        // Store current cursor position.
        int x = getcurx(pad);
        int y = getcury(pad);
        // mvwprintw(pad, 7, 0, "%s", pt.toString().c_str());
        int xMax, yMax; 
        getmaxyx(pad, yMax, xMax);
        chtype colorPair = COLOR_PAIR(unsavedChanges ? unsavedColor : savedColor);
        wattron(pad, colorPair);
        mvwprintw(pad, yMax - 1, 0, " %s %s ", unsavedChanges ? "Unsaved changes to" : "Saved", pt.getFilePath());
        wattroff(pad, colorPair);
        wclrtoeol(pad);
        // Restore old cursor position.
        wmove(pad, y, x);

        // // refreshes the screen
        prefresh(pad, 0, 0, 0, 0, yMax - 1, xMax - 1);

        // read in a character at a time
        int inputChar = wgetch(pad);

        if (inputChar == CTRL_C) {
            break;
        }

        // unsavedChanges = true;
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
                // unsavedChanges = false;
        }

        if (isalnum(inputChar) || ispunct(inputChar) || isspace(inputChar)) {
            pt.insertChar(inputChar);
        }
    }

    delwin(pad);
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}