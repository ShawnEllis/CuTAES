#include "WindowUtil.h"
#include "CuTAES.h"
#include "Button.h"
#include "Panel.h"

using namespace std;

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
