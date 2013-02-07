#include "StartScreen.h"
#include <stdlib.h>

#include "Button.h"
#include "CourseList.h"
#include "WindowUtil.h"

StartScreen* StartScreen::m_pInstance = 0;

StartScreen* StartScreen::instance() {
    if (!m_pInstance) {
        m_pInstance = new StartScreen();
    }
    return m_pInstance;
}

StartScreen::StartScreen() : Panel("Start Screen") {
    //Create buttons
    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 3);
    pButton->setEventHandler(handleCreatePressed);
    this->add(pButton);

    pButton = new Button(this, "Exit", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
    pButton->setEventHandler(handleExitPressed);
    this->add(pButton);
}

StartScreen::~StartScreen() {
}

//Event Handlers

void StartScreen::handleExitPressed() {
    exit(0);
}

void StartScreen::handleCreatePressed() {
    CourseList::instance()->show();
}
