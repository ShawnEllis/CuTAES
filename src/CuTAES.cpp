#include "CuTAES.h"
#include <curses.h>
#include <stdlib.h>

#include "StartScreen.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
std::ofstream dout;
#endif //DEBUG

#include "FormDialog.h"
#include "CourseInfoScreen.h"

const int CuTAES::DEF_W = 80; //Default width of menus
const int CuTAES::DEF_H = 24; //Default height of menus

const int CuTAES::KEY_ENT = 10;

CuTAES* CuTAES::m_pInstance = 0;

CuTAES* CuTAES::instance() {
    if (!m_pInstance) {
        m_pInstance = new CuTAES();
    }
    return m_pInstance;
}

CuTAES::CuTAES() {
    m_workingDirectory = "/Users/zdawson/Library/Developer/Xcode/DerivedData/shared/Products/Debug/";//"./";
#ifdef DEBUG
    dout.open((m_workingDirectory + "debugOut.txt").data());
    dout << "Start CuTAES" << std::endl;
#endif //DEBUG
    atexit(onExit); //Set onExit() to be called on exit
    
    initscr(); //Intialize ncurses
    keypad(stdscr, TRUE); //Enable arrow keys
    cbreak(); //Disable line buffering
    noecho();
    curs_set(0); //Hide cursor
    
    refresh();
}

void CuTAES::setWorkingDirectory(const std::string &dir) {
    m_workingDirectory = dir;
#ifdef DEBUG
    dout << "Working directory: " << CuTAES::instance()->getWorkingDirectory() << std::endl;
#endif //DEBUG
}

void CuTAES::onExit() {
    endwin();
#ifdef DEBUG
    dout.close();
#endif //DEBUG
}

int main(int argc, const char* argv[]) {
    CuTAES::instance();
    if (argc == 2) {
        CuTAES::instance()->setWorkingDirectory(argv[1]); //Init working directory
    }
//    StartScreen::instance()->show();
    
    CourseInfoScreen *courseInfoScr = new CourseInfoScreen();
    courseInfoScr->show();
    return 0;
}
