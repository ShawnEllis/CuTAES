#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <curses.h>
#include "Panel.h"

class Button;

class StartScreen : public Panel {
public:
    static StartScreen* instance();
    virtual ~StartScreen();

    static void handleExitPressed(Button *pButton);
    static void handleCreatePressed(Button *pButton);

private:
    StartScreen();
    static StartScreen* m_pInstance;
};

#endif //STARTSCREEN_H

