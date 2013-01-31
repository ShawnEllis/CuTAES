#include "StartScreen.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include "Button.h"

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
    WindowUtil::createButton(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 3);
    WindowUtil::createButton(this, "Exit", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
}

StartScreen::~StartScreen() {
    delwin(m_pWindow);
}

void StartScreen::show() {
    //Show the window
    Panel::show();
    Panel::waitForInput();
}
