#ifndef BUTTON_H
#define BUTTON_H

#include <curses.h>
#include <string>
#include "Component.h"

class Button : public Component {
public:
	Button(WINDOW *win, const std::string &txt);
    ~Button() {};

    virtual void draw();

private:
    std::string text;

};

#endif //BUTTON_H

