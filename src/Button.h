#ifndef BUTTON_H
#define BUTTON_H

#include <curses.h>
#include <string>
#include "Component.h"

class Panel;

class Button : public Component {
public:
	Button(Panel *pPanel, const std::string &txt, int cx=0, int cy=0);
    ~Button() {};

    virtual void draw();

    virtual void registerActionTriggers();

    virtual void doAction();

    void setEventHandler(void(*handler)()) {eventHandler = handler;}

private:
    std::string text;
    void(*eventHandler)();
};

#endif //BUTTON_H

