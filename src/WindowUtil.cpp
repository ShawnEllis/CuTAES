#include "WindowUtil.h"
#include "CuTAES.h"
#include "Button.h"
#include "Panel.h"

using namespace std;

void WindowUtil::drawHLine(WINDOW *pWindow, int x, int y, int l, int ch) {
    for (int i = x; i < x + l; i++) {
        mvwaddch(pWindow, y, i, ch);
    }
}

void WindowUtil::drawVLine(WINDOW *pWindow, int x, int y, int l, int ch) {
    for (int i = y; i < y + l; i++) {
        mvwaddch(pWindow, i, x, ch);
    }
}

void WindowUtil::drawRect(WINDOW *pWindow, int x, int y, int w, int h,
                          int tl, int tr,
                          int bl, int br,
                          int t,  int b,
                          int l,  int r) {
    drawVLine(pWindow, x,         y        , h, l);
    drawVLine(pWindow, x + w - 1, y        , h, r);
    drawHLine(pWindow, x,         y        , w, t);
    drawHLine(pWindow, x,         y + h - 1, w, b);
    mvwaddch(pWindow, y,         x,         tl);
    mvwaddch(pWindow, y + h - 1, x,         bl);
    mvwaddch(pWindow, y,         x + w - 1, tr);
    mvwaddch(pWindow, y + h - 1, x + w - 1, br);
}

void WindowUtil::fillRect(WINDOW *pWindow, int x, int y, int w, int h, int ch) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            mvwaddch(pWindow, j, i, ch);
        }
    }
}
