/*
 *  MenuStartAdmin.h
 *
 *  The starting menu for admins' actions.
 *
 */

#ifndef MENU_START_ADMIN_H
#define MENU_START_ADMIN_H

#include <curses.h>
#include "Panel.h"

class Button;

class MenuStartAdmin : public Panel {
public:
    static MenuStartAdmin* instance();
    virtual ~MenuStartAdmin();
    
    static void handleBackPressed(Button *pButton);
    static void handleSummaryPressed(Button *pButton);
    
private:
    MenuStartAdmin();
    static MenuStartAdmin* m_pInstance;
};

#endif //MENU_START_ADMIN_H
