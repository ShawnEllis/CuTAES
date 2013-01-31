#include "Panel.h"
#include "Component.h"
#include "ListNode.h"
#include "ActionTrigger.h"
#include <stdio.h>

Panel::Panel() {
    m_componentList = *(new List<Component*>());
    m_actionTriggerList = *(new List<ActionTrigger*>());
}

Panel::~Panel() {
}

void Panel::show() {
    ListNode<Component*>* cur = m_componentList.first();
    while (cur != 0) {
        cur->data->draw();
        cur = cur->pNext;
    }
    wrefresh(m_pWindow);
}

void Panel::waitForInput() {
    while (true) {
        int c = getch();
        if (c == KEY_UP) {
            //Select prev item
            pSelComponent->setSelected(false);
            if (pSelNode->pPrev != 0) {
                pSelNode = pSelNode->pPrev;
            } else {
                pSelNode = m_componentList.last();
            }
            pSelComponent = pSelNode->data;
            pSelComponent->setSelected(true);
            show();
        } else if (c == KEY_DOWN) {
            //Select next item
            pSelComponent->setSelected(false);
            if (pSelNode->pNext != 0) {
                pSelNode = pSelNode->pNext;
            } else {
                pSelNode = m_componentList.first();
            }
            pSelComponent = pSelNode->data;
            pSelComponent->setSelected(true);
            show();
        } else {
            //Iterate through action triggers
            ListNode<ActionTrigger*>* cur = m_actionTriggerList.first();
            while (cur != 0) {
                char buffer[50];
                int n= sprintf(buffer, "%d %d %d", cur->data->trigger, c, KEY_ENTER);
                mvwprintw(getWindow(), 0, 0, buffer);
                wrefresh(getWindow());
                if (c == cur->data->trigger) {
                    //Call action handler
                    (cur->data->pComponent->*(cur->data->action))();
                }
                cur = cur->pNext;
            }

        }
    }
}

void Panel::add(Component *c) {
    m_componentList.addBack(c);
    c->registerActionTriggers();
    if (pSelComponent == 0 && c->isSelectable()) {
        pSelNode = m_componentList.last();
        pSelComponent = c;
        pSelComponent->setSelected(true);
    }
}

void Panel::registerAction(int t, Component *c, void(Component::*a)()) {
    m_actionTriggerList.addBack(new ActionTrigger(t, c, a));
}

