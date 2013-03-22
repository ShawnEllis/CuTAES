#include "MenuCreateApplication.h"
#include <stdlib.h>

#include "MenuWorkExperience.h"
#include "DialogYesNo.h"
#include "Table.h"
#include "Label.h"
#include "TaApplication.h"
#include "Queue.h"
#include "StringUtil.h"

/*
 * Constructor for create-style menu
 */
MenuCreateApplication::MenuCreateApplication(const std::string& course, const std::string& student, bool isGraduate)
: Panel("Create " + course + " Application: Enter Course Info", 70), m_strCourse(course), m_strStudentID(student) {
    setReturnState(STATE_ERROR);
    m_pApplication = 0;
    init(isGraduate);
    m_pWorkExperienceMenu = new MenuWorkExperience("Create " + m_strCourse + " Application: Enter Work Experience");
    if (pRelatedCoursesTable != 0) {
        add(pRelatedCoursesTable);
    }
    add(pTaCoursesTable);
}

/*
 * Constructor for edit-style menu
 */
MenuCreateApplication::MenuCreateApplication(TaApplication *pApp, bool isGraduate) : Panel("Edit Application: Enter Course Info", 70) {
    setReturnState(STATE_ERROR);
    if (pApp == 0) {
        return;
    }
    m_pApplication = pApp;
    m_strCourse = pApp->getCourse();
    m_strStudentID = pApp->getStudentID();
    init(isGraduate);
    //Add related courses to table
    {
        Node<TaApplication::RelatedCourse>* pCur = pApp->getRelatedCourses();
        while (pCur != 0) {
            std::string pData[4] = {pCur->value.m_course, StringUtil::itos(pCur->value.m_year), "", pCur->value.m_grade};
            pData[2] = pCur->value.m_term;
            pRelatedCoursesTable->addRow(pData);
            pCur = pCur->m_pNext;
        }
    }
    //Add ta'd courses to table
    {
        Node<TaApplication::TaCourse>* pCur = pApp->getTaCourses();
        while (pCur != 0) {
            std::string pData[4] = {pCur->value.m_course, StringUtil::itos(pCur->value.m_year), "", pCur->value.m_supervisor};
            pData[2] = pCur->value.m_term;
            pTaCoursesTable->addRow(pData);
            pCur = pCur->m_pNext;
        }
    }
    m_pWorkExperienceMenu = new MenuWorkExperience("Edit Application: Enter Work Experience", pApp);
    if (pRelatedCoursesTable != 0) {
        add(pRelatedCoursesTable);
    }
    add(pTaCoursesTable);
    
    add(new Label(this, "Close Application: F5", 1, getHeight() - 3));
}

/*
 * Init components
 */
void MenuCreateApplication::init(bool isGraduate) {
    //Create related courses table
    if (!isGraduate) {
        std::string labels[] = {"Course", "Year", "Term", "Grade"};
        int colWidths[] = {8, 4, 4, 5};
        pRelatedCoursesTable = new Table(this, 3, 4, 15, 4, colWidths, labels);
    } else {
        pRelatedCoursesTable = 0;
    }
    //Create TA info table
    std::string labels[] = {"Course", "Year", "Term", "Supervisor"};
    int colWidths[] = {8, 4, 4, 16};
    int x = isGraduate ? getWidth()/2-18 : pRelatedCoursesTable->getX() +  pRelatedCoursesTable->getWidth() + 1;
    pTaCoursesTable = new Table(this, x, 4, 15, 4, colWidths, labels);
    
    //Create labels
    if (!isGraduate) {
        add(new Label(this, "Related Courses", pRelatedCoursesTable->getX(), 3));
    }
    add(new Label(this, "Related Courses TA'd", pTaCoursesTable->getX(), 3));
    add(new Label(this, "Cancel: F3", 1, getHeight() - 2));
    add(new Label(this, "Continue: F2", getWidth() - 13, getHeight() - 2));
}

MenuCreateApplication::~MenuCreateApplication() {
    //TODO: dealloc memory
}

bool MenuCreateApplication::handleKeyPress(int key) {
    if (key == KEY_F(2)) {
        //TODO: verify
        StateType state = m_pWorkExperienceMenu->show();
        if (state == STATE_SUCCESS) {
            setReturnState(STATE_SUCCESS);
        }
        hide();
        return true;
    } else if (key == KEY_F(3)) {
        setReturnState(STATE_CANCEL);
        hide();
        return true;
    } else if (key == KEY_F(5)) {
        if (m_pApplication != 0) {
            m_pApplication->setStatus(STATUS_CLOSED);
            m_pApplication->saveToFile();
            setReturnState(STATE_CANCEL);
            hide();
            DialogYesNo* pDia = new DialogYesNo("Application closed.", DIALOG_MESSAGE);
            pDia->show();
            delete pDia;
            return true;            
        }
    }
    return false;
}

/*
 * Creates a new TaApplication with the data inputted by the user.
 * pApplication must be a pointer to null.
 */
bool MenuCreateApplication::getData(TaApplication **pApplication) {
    if (getReturnState() != STATE_SUCCESS || m_pWorkExperienceMenu->getReturnState() != STATE_SUCCESS
        || pApplication == 0 || *pApplication != 0) {
        return false;
    }
    *pApplication = new TaApplication(m_strCourse, m_strStudentID);
    std::string *pData;
    
    //Save Related Courses data to application
    if (pRelatedCoursesTable != 0) {
        for (int i = 0 ; i < pRelatedCoursesTable->getNumRows(); i++) {
            pData = 0;
            pRelatedCoursesTable->getDataInRow(i, &pData);
            //Create RelatedCourse using row data
            (*pApplication)->addRelatedCourse(pData[0], atoi(pData[1].data()), pData[2][0], pData[3]);
        }
    }
    
    //Save TA'd courses data to application
    for (int i = 0 ; i < pTaCoursesTable->getNumRows(); i++) {
        pData = 0;
        pTaCoursesTable->getDataInRow(i, &pData);
        //Create RelatedCourse using row data
        (*pApplication)->addTaCourse(pData[0], atoi(pData[1].data()), pData[2][0], pData[3]);
    }
    
    m_pWorkExperienceMenu->getData(*pApplication);
    
    return true;
}