#ifndef COMPONENT_H
#define COMPONENT_H

#include <curses.h>

class Panel;

class Component {
public:
    
    /*
     * Create a new component using pPanel for rendering & events
     * TODO: Remove panel arg
     */
    Component(Panel *pPanel, int x=0, int y=0, int w=1, int h=1);
    
    virtual ~Component();

    /*
     * Draw the component to m_pPanel
     */
    virtual void draw() = 0;

    /*
     * Handle a key event.
     * Return true to consume the event.
     */
    virtual bool handleKeyPress(int ch) {return false;}

    //Getters & Setters
    
    int getX() {return m_x;}
    void setX(int x) {m_x = x;}
    
    int getY() {return m_y;}
    void setY(int y) {m_y = y;}
    
    int getWidth() {return m_width;}
    void setWidth(int w) {m_width = w;}
    
    int getHeight() {return m_height;}
    void setHeight(int h) {m_height = h;}

    bool isSelectable() {return m_selectable;}
    bool isSelected() {return m_selected;}
    virtual void setSelected(bool sel) { if (m_selectable) { m_selected = sel; } }
    
    void* getUsrPtr() {return m_usrPtr;}
    void setUsrPtr(void* p) {m_usrPtr = p;}

protected:
    Panel *m_pPanel;
    
    void setSelectable(bool sel) {m_selectable = sel; m_selected = false;}
    
private:
    int m_x, m_y;
    int m_width, m_height;
    
    bool m_selectable;
    bool m_selected;
    
    void *m_usrPtr;
    
};

#endif //COMPONENT_H
