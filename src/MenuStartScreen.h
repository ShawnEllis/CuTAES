/*
 *  MenuStartScreen.h
 *
 *  The starting menu for the application.
 *
 */

#ifndef MENU_STARTSCREEN_H
#define MENU_STARTSCREEN_H

#include <curses.h>
#include "Panel.h"

class Button;

class MenuStartScreen : public Panel {
public:
    static MenuStartScreen* instance();
    virtual ~MenuStartScreen();
    
    static void handleExitPressed(Button *pButton);
    static void handleUndergradPressed(Button *pButton);
    static void handleGradPressed(Button *pButton);
    static void handleAdminPressed(Button *pButton);
    
private:
    MenuStartScreen();
    static MenuStartScreen* m_pInstance;
};

#endif //MENU_STARTSCREEN_H
