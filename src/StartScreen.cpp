#include "StartScreen.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include "Button.h"
#include "CourseList.h"
#include <stdlib.h>

StartScreen* StartScreen::m_pInstance = 0;

StartScreen* StartScreen::instance() {
    if (!m_pInstance) {
        m_pInstance = new StartScreen();
    }
    return m_pInstance;
}

StartScreen::StartScreen() {
    WindowUtil::createTitledWindow(&m_pWindow, "Start Screen");
    //Create buttons

    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 3);
    pButton->setEventHandler(handleCreatePressed);
    this->add(pButton);

    pButton = new Button(this, "Exit", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
    pButton->setEventHandler(handleExitPressed);
    this->add(pButton);
}

StartScreen::~StartScreen() {
    delwin(m_pWindow);
}

void StartScreen::show() {
    //Show the window
    Panel::show();
    Panel::waitForInput();
}

//Event Handlers

void StartScreen::handleExitPressed() {
    exit(0);
}

void StartScreen::handleCreatePressed() {
    CourseList::instance()->show();
}
