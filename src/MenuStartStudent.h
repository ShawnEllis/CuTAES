#ifndef MENU_STUDENT_START_H
#define MENU_STUDENT_START_H

#include <curses.h>
#include "Panel.h"

class Button;

class MenuStartStudent : public Panel {
public:
    static MenuStartStudent* instance();
    virtual ~MenuStartStudent();

    static void handleBackPressed(Button *pButton);
    static void handleCreatePressed(Button *pButton);

private:
    MenuStartStudent();
    static MenuStartStudent* m_pInstance;
};

#endif //MENU_STUDENT_START_H

