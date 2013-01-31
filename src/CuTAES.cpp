#include "CuTAES.h"
#include <curses.h>
#include <stdlib.h>

#include "StartScreen.h"

const int CuTAES::DEF_W = 80;
const int CuTAES::DEF_H = 24;
const int CuTAES::KEY_ENT = 10;

CuTAES* CuTAES::m_pInstance = 0;

CuTAES* CuTAES::instance() {
    if (!m_pInstance) {
        m_pInstance = new CuTAES();
    }
    return m_pInstance;
}

CuTAES::CuTAES() {
    atexit(onExit);
    initscr();
    keypad(stdscr, TRUE); //Enable arrow keys
    cbreak(); //Disable line buffering
    noecho();
    curs_set(0); //Hide cursor
    refresh();

    StartScreen::instance()->show();
}

void CuTAES::onExit() {
    endwin();
}

int main() {
    CuTAES::instance();
    return 0;
}
