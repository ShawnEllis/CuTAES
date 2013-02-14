#ifndef BUTTON_H
#define BUTTON_H

#include <curses.h>
#include <string>
#include "Component.h"

class Panel;

class Button : public Component {
public:
	Button(Panel *pPanel, const std::string &txt, int cx=0, int cy=0, int borderX = 6, int borderY = 2);
    ~Button() {};

    virtual void draw();

    virtual bool handleKeyPress(int ch);

    void setEventHandler(void(*handler)(Button *pButton)) {eventHandler = handler;}
    
    const std::string& getText() {return m_text;}

    bool isEnabled() {return m_enabled;}
    void setEnabled(bool enabled) {m_enabled = enabled; setSelectable(enabled);}
    
private:
    std::string m_text;    
    int m_borderX, m_borderY;
    
    bool m_enabled;
    
    void(*eventHandler)(Button *pButton);
};

#endif //BUTTON_H

