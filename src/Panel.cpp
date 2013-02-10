#include "Panel.h"
#include <stdio.h>

#include "Component.h"
#include "ListNode.h"
#include "ActionTrigger.h"
#include "WindowUtil.h"


#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

using namespace std;

Panel::Panel(const string &t, int w, int h) : m_title(t) {
    pSelComponent = 0;
    pSelNode = 0;
    m_pWindow = newwin(h, w, 0, 0);
    m_componentList = *(new List<Component*>());
    wrefresh(m_pWindow);
}

Panel::~Panel() {
    delwin(m_pWindow);
}

void Panel::show() {
#ifdef DEBUG
    dout << "Show panel " << m_title << endl;
#endif //DEBUG
    
    m_visible = true;
    draw();
    waitForInput();
}

void Panel::draw() {
    
    //Decorate the window
    box(m_pWindow, 0 , 0);
    mvwprintw(m_pWindow, 1, (CuTAES::DEF_W - m_title.length()) / 2, m_title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, CuTAES::DEF_W - 2);
    
    drawComponents();
    
    wrefresh(m_pWindow);
}

/*
 * Iterate through componentList and draw components
 */
void Panel::drawComponents() {
    ListNode<Component*>* cur = m_componentList.first();
    while (cur != 0) {
        cur->data->draw();
        cur = cur->pNext;
    }
}

void Panel::waitForInput() {
    while (m_visible) {
//        draw();
        wrefresh(m_pWindow);
        int ch = getch();
        if (pSelComponent != 0 && pSelComponent->handleKeyPress(ch)) {
            continue;
        }
        if (handleKeyPress(ch)) {
            continue;
        }
        if (m_componentList.getSize() > 1) {
            if (ch == KEY_UP) {
                //Select prev item
                pSelComponent->setSelected(false);
                if (pSelNode->pPrev != 0) {
                    pSelNode = pSelNode->pPrev;
                } else {
                    pSelNode = m_componentList.last();
                }
                pSelComponent = pSelNode->data;
                pSelComponent->setSelected(true);
                draw();
            } else if (ch == KEY_DOWN) {
                //Select next item
                pSelComponent->setSelected(false);
                if (pSelNode->pNext != 0) {
                    pSelNode = pSelNode->pNext;
                } else {
                    pSelNode = m_componentList.first();
                }
                pSelComponent = pSelNode->data;
                pSelComponent->setSelected(true);
                draw();
            }
        }
    }
}

void Panel::add(Component *c) {
    m_componentList.addBack(c);
    if (pSelComponent == 0 && c->isSelectable()) {
        pSelNode = m_componentList.last();
        pSelComponent = c;
        pSelComponent->setSelected(true);
    }
}

