#include "main.hpp"
#include <ncurses.h>
#include "piece_table.hpp"

int main() {
    // init screen and sets up screen
    initscr();

    // print to screen
    printw("Hello World");

    // refreshes the screen
    refresh();

    // pause the screen output
    getch();

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}