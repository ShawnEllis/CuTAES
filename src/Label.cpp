#include "Label.h"

#include "Panel.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

Label::Label(Panel *pPanel, const std::string& text, int x, int y) : Component(pPanel, x, y, 1, 1) {
    m_text = text;
}

Label::~Label() {
}

void Label::draw() {
    mvwprintw(m_pPanel->getWindow(), getY(), getX(), m_text.data());
#ifdef DEBUG
    dout << "Label::draw() - text=" << m_text << " x=" << getX() << " y=" << getY() << std::endl;
#endif //DEBUG
}