#include "CourseInfoScreen.h"

//#include "ListBox.h"
#include "Table.h"
#include "Label.h"

using namespace std;

CourseInfoScreen::CourseInfoScreen() : Panel("Course Info Screen", 66) {
    string labels[] = {"Course", "Year", "Term", "Grade"};
    int colWidths[] = {8, 4, 4, 5};
    Table *pTable = new Table(this, 3, 4, 16, 4, colWidths, labels);
    add(pTable);
    
    Label* pLabel = new Label(this, "Related Courses", pTable->getX(), 3);
    add(pLabel);
    
    string labels2[] = {"Course", "Year", "Term", "Supervisor"};
    int colWidths2[] = {8, 4, 4, 10};
    pTable = new Table(this, pTable->getX() + pTable->getWidth(), 4, 16, 4, colWidths2, labels2);
    add(pTable);
    
    pLabel = new Label(this, "Related Courses TA'd", pTable->getX(), 3);
    add(pLabel);
}

CourseInfoScreen::~CourseInfoScreen() {
}

