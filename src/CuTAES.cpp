#include "cuTAES.h"
#include <curses.h>

#include "StartScreen.h"

const int CuTAES::DEF_W = 80;
const int CuTAES::DEF_H = 24;

int main() {
    initscr();
    keypad(stdscr, TRUE); //Enable arrow keys
    cbreak(); //Disable line buffering
    noecho();
    curs_set(0); //Hide cursor

    printw("Hello World !!!");
    refresh();

    getch();

    StartScreen::instance()->show();

    endwin();
    return 0;
}
