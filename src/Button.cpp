#include "Button.h"
#include "WindowUtil.h"

using namespace std;

const int BORDER_SIZE_X = 6;
const int BORDER_SIZE_Y = 2;

Button::Button(WINDOW *win, const string &txt) : Component(win), text(txt) {
    x = 0;
    y = 0;
    width = text.size() + BORDER_SIZE_X * 2;
    height = 1 + BORDER_SIZE_Y * 2;
    m_selected = false;
}

void Button::draw() {
    WindowUtil::fillRect(pWindow, x, y, width, height, m_selected ? '-' : ' ');
    WindowUtil::drawRect(pWindow, x, y, width, height);
    mvwprintw(pWindow, y + BORDER_SIZE_Y, x + BORDER_SIZE_X, text.data());
}
