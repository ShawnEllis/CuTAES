#include "Component.h"
#include "Panel.h"

Component::Component(Panel *pPanel) : pPanel(pPanel) {
    m_selectable = true;
}

Component::~Component() {

}

void Component::doAction() {
}
