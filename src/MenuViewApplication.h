#ifndef MENU_VIEW_APPLICATION_H
#define MENU_VIEW_APPLICATION_H

#include <curses.h>
#include "Panel.h"

class Button;

class MenuViewApplication : public Panel {
public:
    MenuViewApplication();
    virtual ~MenuViewApplication();
    
    static void handleSearchAppPressed(Button *pButton);
    static void handleSearchStuPressed(Button *pButton);
    static void handleSearchNamePressed(Button *pButton);
    static void handleBackPressed(Button *pButton);
};

#endif //MENU_VIEW_APPLICATION_H

