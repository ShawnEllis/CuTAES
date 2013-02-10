#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <curses.h>
#include "Panel.h"

class StartScreen : public Panel {
public:
    static StartScreen* instance();
    virtual ~StartScreen();

    static void handleExitPressed();
    static void handleCreatePressed();

private:
    StartScreen();
    static StartScreen* m_pInstance;
};

#endif //STARTSCREEN_H

