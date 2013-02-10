#include "CourseInfoScreen.h"

#include "ListBox.h"
#include "Label.h"

CourseInfoScreen::CourseInfoScreen() : Panel("Course Info Screen") {
//    curs_set(1);
    ListBox *pList = new ListBox(this, 1, 4, 20, 10);
    add(pList);
    
    Label* pLabel = new Label(this, "asd", 1, 3);
    add(pLabel);
}

CourseInfoScreen::~CourseInfoScreen() {
//    curs_set(0);
}

