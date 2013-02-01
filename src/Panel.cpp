#include "Panel.h"
#include "Component.h"
#include "CuTAES.h"
#include "ListNode.h"
#include "ActionTrigger.h"
#include "WindowUtil.h"
#include <stdio.h>

using namespace std;

Panel::Panel(const string &t) : title(t) {
    m_pWindow = newwin(CuTAES::DEF_H, CuTAES::DEF_W, 0, 0);
    m_componentList = *(new List<Component*>());
    m_actionTriggerList = *(new List<ActionTrigger*>());
    wprintw(m_pWindow, title.data());
    wrefresh(m_pWindow);
}

Panel::~Panel() {
    delwin(m_pWindow);
}

void Panel::show() {
    wclear(m_pWindow);
    //Decorate the window
    box(m_pWindow, 0 , 0);
    mvwprintw(m_pWindow, 1, (CuTAES::DEF_W - title.length()) / 2, title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, CuTAES::DEF_W - 2);
    ListNode<Component*>* cur = m_componentList.first();
    while (cur != 0) {
        cur->data->draw();
        cur = cur->pNext;
    }
    draw();
    wrefresh(m_pWindow);
    waitForInput();
}

void Panel::waitForInput() {
    while (true) {
        int c = getch();
        if (!handleKeyPress(c)) { //Event is not consumed
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
                    #ifdef DEBUG
                    char buffer[50];
                    sprintf(buffer, "%d %d %d", cur->data->trigger, c, KEY_ENTER);
                    mvwprintw(getWindow(), 0, 0, buffer);
                    wrefresh(getWindow());
                    #endif //DEBUG
                    if (c == cur->data->trigger) {
                        //Call action handler
                        (cur->data->pComponent->*(cur->data->action))();
                    }
                    cur = cur->pNext;
                }

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

