#ifndef STARTSCREEN_H_
#define STARTSCREEN_H_

#include <curses.h>
#include "Panel.h"

class StartScreen : public Panel {
public:
    static StartScreen* instance();
    virtual ~StartScreen();

    virtual void draw();

    static void handleExitPressed();
    static void handleCreatePressed();

private:
    StartScreen();
    static StartScreen* m_pInstance;
};

#endif //STARTSCREEN_H_

