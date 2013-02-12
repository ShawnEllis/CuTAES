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

    virtual bool handleKeyPress(int ch);

    void setEventHandler(void(*handler)(Button *pButton)) {eventHandler = handler;}
    
    const std::string& getText() {return m_text;}

private:
    std::string m_text;
    void(*eventHandler)(Button *pButton);
};

#endif //BUTTON_H

