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
    Button *pButton = new Button(this, "Undergrads", getWidth() / 2, getHeight() / 2 - 7, 5);
    pButton->setEventHandler(handleUndergradPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Grad Students ", getWidth() / 2, pButton->getY() + pButton->getHeight() + 2, 3);
    pButton->setEventHandler(handleGradPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Administrators", getWidth() / 2, pButton->getY() + pButton->getHeight() + 2, 3);
    pButton->setEventHandler(handleAdminPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Exit", getWidth() / 2, pButton->getY() + pButton->getHeight() + 2);
    pButton->setEventHandler(handleExitPressed);
    this->add(pButton);
}

MenuStartScreen::~MenuStartScreen() {
}

//Event Handlers

void MenuStartScreen::handleExitPressed(Button *pButton) {
    MenuStartScreen::instance()->hide();
}

void MenuStartScreen::handleUndergradPressed(Button *pButton) {
    MenuStartScreen::instance()->erase();
    MenuStartStudent *pMenu = new MenuStartStudent(true);
    pMenu->show();
    delete pMenu;
    MenuStartScreen::instance()->draw();
}

void MenuStartScreen::handleGradPressed(Button *pButton) {
    MenuStartScreen::instance()->erase();
    MenuStartStudent *pMenu = new MenuStartStudent(false);
    pMenu->show();
    delete pMenu;
    MenuStartScreen::instance()->draw();
}

void MenuStartScreen::handleAdminPressed(Button *pButton) {
    MenuStartScreen::instance()->erase();
    MenuStartAdmin::instance()->show();
    MenuStartScreen::instance()->draw();    
}
