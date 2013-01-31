#ifndef COMPONENT_H
#define COMPONENT_H

#include <curses.h>

class Panel;

class Component {
public:
    Component(Panel *pPanel);
    virtual ~Component();

    virtual void draw() = 0;

    /*
        Registers event triggers on pPanel.
        Useful for components with actions;
          eg. button uses enter as trigger for doing an action
    */
    virtual void registerActionTriggers() = 0;

    virtual void doAction();

    int getX() {return x;}
    void setX(int x) {this->x = x;}
    int getY() {return y;}
    void setY(int y) {this->y = y;}
    int getWidth() {return width;}
    void setWidth(int w) {width = w;}
    int getHeight() {return height;}
    void setHeight(int h) {width = h;}

    bool isSelectable() {return m_selectable;}
    bool isSelected() {return m_selected;}
    void setSelected(bool sel) { if (m_selectable) { m_selected = sel; } }


protected:
    int x, y;
    int width, height;
    bool m_selectable;
    bool m_selected;

    Panel *pPanel;
};

#endif //COMPONENT_H
