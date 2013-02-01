#ifndef PANEL_H
#define PANEL_H

#include <curses.h>
#include "List.h"
#include <string>

template <class T>
class ListNode;
class Component;
class ActionTrigger;

class Panel {
public:
    Panel(const std::string &t);
    virtual ~Panel();

    virtual void show();
    virtual void draw() {} //used by subclasses to draw special decorations

    virtual void waitForInput();

    /*
        Used by subclasses to handle special input.
        Return true to consume the event.
    */
    virtual bool handleKeyPress(int key) {return false;} 

    void add(Component *c);

    void registerAction(int t, Component *c, void(Component::*a)());

    WINDOW* getWindow() {return m_pWindow;}

protected:
    WINDOW *m_pWindow;
    std::string title;

    List<Component*> m_componentList;
    ListNode<Component*> *pSelNode;
    Component* pSelComponent;

    List<ActionTrigger*> m_actionTriggerList;
};

#endif //PANEL_H

