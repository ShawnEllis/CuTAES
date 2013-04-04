#include "MenuWorkExperience.h"
#include <stdlib.h>

#include "Table.h"
#include "Label.h"
#include "TaApplication.h"
#include "Queue.h"

MenuWorkExperience::MenuWorkExperience(const std::string& title, TaApplication* pApp, bool editable) : Panel(title, 70) {
    setReturnState(STATE_ERROR);
    std::string labels[] = {"Job", "Description", "Start Date", "End Date"};
    int colWidths[] = {16, 32, 8, 8};
    
    m_pWorkExperienceTable = new Table(this, 1, 4, 16, 4, colWidths, labels, editable);
    if (pApp != 0) {
        //Add work exp. to table
        {
            Node<TaApplication::WorkExperience>* pCur = pApp->getWorkExperience();
            while (pCur != 0) {
                std::string pData[4] = {pCur->value.m_job, pCur->value.m_description, pCur->value.m_start, pCur->value.m_end};
                m_pWorkExperienceTable->addRow(pData);
                pCur = pCur->m_pNext;
            }
        }
    }
    add(m_pWorkExperienceTable);
    
    add(new Label(this, "Work Experience", m_pWorkExperienceTable->getX(), 3));
    add(new Label(this, "Cancel: F3", 1, getHeight() - 2));
    add(new Label(this, "Continue: F2", getWidth() - 13, getHeight() - 2));
}

MenuWorkExperience::~MenuWorkExperience() {
    //TODO: dealloc memory
}

bool MenuWorkExperience::handleKeyPress(int key) {
    if (key == KEY_F(2)) {
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
 * pApplication must be a valid application.
 */
bool MenuWorkExperience::getData(TaApplication *pApplication) {
    if (getReturnState() != STATE_SUCCESS || pApplication == 0) {
        return false;
    }
    std::string *pData;
    
    //Save data to application
    for (int i = 0 ; i < m_pWorkExperienceTable->getNumRows(); i++) {
        pData = 0;
        m_pWorkExperienceTable->getDataInRow(i, &pData);
        //Create RelatedCourse using row data
        pApplication->addWorkExperience(pData[0], pData[1].data(), pData[2], pData[3]);
    }
    
    return true;
}