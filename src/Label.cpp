#include "Label.h"

#include "Panel.h"

Label::Label(Panel *pPanel, const std::string& text, int x, int y, bool overlay) : Component(pPanel, x, y, 1, 1) {
    m_text = text;
    m_overlay = overlay;
}

Label::~Label() {
}

void Label::draw() {
    if (!m_overlay) {
        mvwprintw(m_pPanel->getWindow(), getY(), getX(), m_text.data());
    } else {
        mvprintw(getY(), getX(), m_text.data());
    }
}