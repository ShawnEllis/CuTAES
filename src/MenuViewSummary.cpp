#include "MenuViewSummary.h"
#include "CuTAES.h"
#include "Label.h"
#include "Table.h"

MenuViewSummary::MenuViewSummary(const std::string& title) : Panel(title, 143, 25) {
    setReturnState(STATE_ERROR);

    std::string labels[] = {"Student ID", "First", "Last", "Email", "Major", "Year Standing", "Major GPA", "CGPA"};
    int colWidths[] = {10, 16, 16, 32, 32, 13, 9, 4};
    Table *pTable = new Table(this, 1, 3, 10, 8, colWidths, labels);
    pTable->setEditable(false);
    add(pTable);
    
    add(new Label(this, "Enter: Back", 1, getHeight() - 2));
}

MenuViewSummary::~MenuViewSummary() {
    
}

/*
 Handle left and right movement
 */
bool MenuViewSummary::handleKeyPress(int key) {
    if (key == CuTAES::KEY_ENT) {
        hide();
        return true;
    }
    return false;
}