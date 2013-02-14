#include "MenuStartScreen.h"

#include "Button.h"
#include "MenuStartStudent.h"
#include "MenuStartAdmin.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

MenuStartScreen* MenuStartScreen::m_pInstance = 0;

MenuStartScreen* MenuStartScreen::instance() {
    if (!m_pInstance) {
        m_pInstance = new MenuStartScreen();
    }
    return m_pInstance;
}

MenuStartScreen::MenuStartScreen() : Panel("CuTAES- Carleton University TA Application & Enrollment System") {
    //Create buttons
    Button *pButton = new Button(this, "Students", getWidth() / 2, getHeight() / 2 - 6);
    pButton->setEventHandler(handleStudentPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Administrators", getWidth() / 2, pButton->getY() + pButton->getHeight() + 3, 3);
    pButton->setEventHandler(handleAdminPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Exit", getWidth() / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEventHandler(handleExitPressed);
    this->add(pButton);
}

MenuStartScreen::~MenuStartScreen() {
}

//Event Handlers

void MenuStartScreen::handleExitPressed(Button *pButton) {
    MenuStartScreen::instance()->hide();
}

void MenuStartScreen::handleStudentPressed(Button *pButton) {
    MenuStartScreen::instance()->erase();
    MenuStartStudent::instance()->show();
    MenuStartScreen::instance()->draw();
}

void MenuStartScreen::handleAdminPressed(Button *pButton) {
    MenuStartScreen::instance()->erase();
    MenuStartAdmin::instance()->show();
    MenuStartScreen::instance()->draw();    
}
