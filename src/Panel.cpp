#include "Panel.h"
#include "Component.h"
#include "ListNode.h"

Panel::Panel() {
    m_componentList = *(new List<Component*>());
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
        } else if (c == KEY_ENTER) {
            //Do something
            break;
        } else {
            break;
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
