#ifndef PANEL_H
#define PANEL_H

#include <curses.h>
#include "List.h"

template <class T>
class ListNode;
class Component;
class ActionTrigger;

class Panel {
public:
    Panel();
    virtual ~Panel();

    virtual void show();
    virtual void waitForInput();

    void add(Component *c);

    void registerAction(int t, Component *c, void(Component::*a)());

    WINDOW* getWindow() {return m_pWindow;}

protected:
    WINDOW *m_pWindow;

    List<Component*> m_componentList;
    ListNode<Component*> *pSelNode;
    Component* pSelComponent;

    List<ActionTrigger*> m_actionTriggerList;
};

#endif //PANEL_H

