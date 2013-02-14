#include "CuTAES.h"
#include <curses.h>
#include <stdlib.h>

#include "MenuStartScreen.h"
#include "DialogForm.h"
#include "Student.h"
#include "Database.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
std::ofstream dout;
#endif //DEBUG

const int CuTAES::DEF_W = 80; //Default width of menus
const int CuTAES::DEF_H = 24; //Default height of menus

const int CuTAES::KEY_ENT = 10;

CuTAES* CuTAES::m_pInstance = 0;

CuTAES::CuTAES() {
    m_workingDirectory = "/Users/zdawson/Library/Developer/Xcode/DerivedData/shared/Products/Debug/";//"./";
#ifdef DEBUG
    dout.open((m_workingDirectory + "debugOut.txt").data());
    dout << "Start CuTAES" << std::endl;
#endif //DEBUG
    m_pActiveUser = 0;
    atexit(onExit); //Set onExit() to be called on exit
    
    initscr(); //Intialize ncurses
    keypad(stdscr, TRUE); //Enable arrow keys
    cbreak(); //Disable line buffering
    noecho();
    curs_set(0); //Hide cursor
    
    refresh();
}

Student* CuTAES::getActiveUser() {
    if (m_pActiveUser == 0) {
        //Create student info dialog
        DialogForm *pForm = new DialogForm("Enter Student Info", 8);
        pForm->addField("First Name:   ", 1, 32, FIELDTYPE_ALPHA);
        pForm->addField("Last Name:    ", 1, 32, FIELDTYPE_ALPHA);
        pForm->addField("Student ID:   ", 1, 32, FIELDTYPE_INT);
        pForm->addField("Email:        ", 1, 32);
        pForm->addField("Major:        ", 1, 32);
        pForm->addField("Year Standing:", 1, 2, FIELDTYPE_INT);
        int range[] = {0, 1, 12};// TODO: Investigate why float validation is broken on OS X
        pForm->addField("CGPA:", 1, 4, 20, 10, FIELDTYPE_FLOAT, range, 3);
        pForm->addField("Major GPA:", 1, 4, 32, 10, FIELDTYPE_FLOAT, range, 3);
        
        //Show dialog
        if (pForm->show() != STATE_SUCCESS) {
            delete pForm;
            return 0;
        }
        
        //Get result
        std::string *data = 0;
        if (pForm->getFormData(&data)) {
            //Save student
            m_pActiveUser = new Student(data[0],
                                       data[1],
                                       data[2],
                                       data[3],
                                       data[4],
                                       atoi(data[5].data()),
                                       atof(data[6].data()),
                                       atof(data[7].data()));
            m_pActiveUser->saveToFile();
            Database::instance()->addStudent(m_pActiveUser);
        }
        delete pForm;
    }
    return m_pActiveUser;
}

//Static methods

CuTAES* CuTAES::instance() {
    if (!m_pInstance) {
        m_pInstance = new CuTAES();
    }
    return m_pInstance;
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
    MenuStartScreen::instance()->show();
    
    return 0;
}
