#include "WindowUtil.h"
#include "CuTAES.h"
#include "Button.h"
#include "Panel.h"

using namespace std;

void WindowUtil::createTitledWindow(WINDOW **pWindow, const string &title) {
    //Create the window
    *pWindow = newwin(CuTAES::DEF_H, CuTAES::DEF_W, 0, 0);
    //Decorate the window
    box(*pWindow, 0 , 0);
    mvwprintw(*pWindow, 1, (CuTAES::DEF_W - title.size()) / 2, title.data());
    drawHLine(*pWindow, 1, 2, CuTAES::DEF_W - 2);
}

void WindowUtil::createButton(Panel *pPanel, const string &text, int x, int y) {
    Button *pButton = new Button(pPanel->getWindow(), text);
    pButton->setX(x - (pButton->getWidth() / 2));
    pButton->setY(y - (pButton->getHeight() / 2));
    pPanel->add(pButton);
}

void WindowUtil::drawHLine(WINDOW *pWindow, int x, int y, int l, char c) {
    for (int i = x; i < x + l; i++) {
        mvwaddch(pWindow, y, i, c);
    }
}

void WindowUtil::drawVLine(WINDOW *pWindow, int x, int y, int l, char c) {
    for (int i = y; i < y + l; i++) {
        mvwaddch(pWindow, i, x, c);
    }
}

void WindowUtil::drawRect(WINDOW *pWindow, int x, int y, int w, int h) {
    drawVLine(pWindow, x, y, h);
    drawVLine(pWindow, x + w - 1, y, h);
    drawHLine(pWindow, x, y, w, '-');
    drawHLine(pWindow, x, y + h - 1, w, '-');
    mvwaddch(pWindow, y,         x,         '+');
    mvwaddch(pWindow, y + h - 1, x,         '+');
    mvwaddch(pWindow, y,         x + w - 1, '+');
    mvwaddch(pWindow, y + h - 1, x + w - 1, '+');
}

void WindowUtil::fillRect(WINDOW *pWindow, int x, int y, int w, int h, char c) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            mvwaddch(pWindow, j, i, c);
        }
    }
}
