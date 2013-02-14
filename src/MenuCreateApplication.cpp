#include "MenuCreateApplication.h"
#include <stdlib.h>

#include "MenuCourseSelector.h"
#include "Table.h"
#include "Label.h"
#include "TaApplication.h"

MenuCreateApplication::MenuCreateApplication(const std::string& course, const std::string& student)
: Panel("Create " + course + " Application: Enter Course Info", 70), m_strCourse(course), m_strStudentID(student) {
    setReturnState(STATE_ERROR);
    
    //Create related courses table
    std::string labels[] = {"Course", "Year", "Term", "Grade"};
    int colWidths[] = {8, 4, 4, 5};
    pRelatedCoursesTable = new Table(this, 3, 4, 16, 4, colWidths, labels);
    add(pRelatedCoursesTable);
    
    //Create TA info table
    std::string labels2[] = {"Course", "Year", "Term", "Supervisor"};
    int colWidths2[] = {8, 4, 4, 16};
    pTaCoursesTable = new Table(this, pRelatedCoursesTable->getX() +  pRelatedCoursesTable->getWidth() + 1, 4, 16, 4, colWidths2, labels2);
    add(pTaCoursesTable);
    
    //Create labels
    add(new Label(this, "Related Courses", pRelatedCoursesTable->getX(), 3));
    add(new Label(this, "Related Courses TA'd", pTaCoursesTable->getX(), 3));
    add(new Label(this, "Cancel: F3", 1, getHeight() - 2));
    add(new Label(this, "Continue: F1", getWidth() - 13, getHeight() - 2));
}

MenuCreateApplication::~MenuCreateApplication() {
    //TODO: dealloc memory
}

bool MenuCreateApplication::handleKeyPress(int key) {
    if (key == KEY_F(1)) {
        //TODO: verify
        setReturnState(STATE_SUCCESS);
        hide();
        return true;
    } else if (key == KEY_F(3)) {
        setReturnState(STATE_CANCEL);
        hide();
        return true;
    }
    return false;
}

/*
 * Creates a new TaApplication with the data inputted by the user.
 * pApplication must be a pointer to null.
 */
bool MenuCreateApplication::getData(TaApplication **pApplication) {
    if (getReturnState() != STATE_SUCCESS || pApplication == 0 || *pApplication != 0) {
        return false;
    }
    *pApplication = new TaApplication(m_strCourse, m_strStudentID);
    std::string *pData;
    
    //Save Related Courses data to application
    for (int i = 0 ; i < pRelatedCoursesTable->getNumRows(); i++) {
        pData = 0;
        pRelatedCoursesTable->getDataInRow(i, &pData);
        //Create RelatedCourse using row data
        (*pApplication)->addRelatedCourse(pData[0], std::atoi(pData[1].data()), pData[2][0], pData[3]);
    }
    
    //Save TA'd courses data to application
    for (int i = 0 ; i < pTaCoursesTable->getNumRows(); i++) {
        pData = 0;
        pTaCoursesTable->getDataInRow(i, &pData);
        //Create RelatedCourse using row data
        (*pApplication)->addTaCourse(pData[0], std::atoi(pData[1].data()), pData[2][0], pData[3]);
    }
    
    return true;
}