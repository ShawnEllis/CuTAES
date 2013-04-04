#include "MenuStartAdmin.h"
#include "Button.h"
#include "DialogListSelector.h"
#include "MenuViewSummary.h"
#include "DialogYesNo.h"
#include "MenuViewApplication.h"

#include "Database.h"
#include "TaApplication.h"
#include "Queue.h"
#include "Student.h"

MenuStartAdmin* MenuStartAdmin::m_pInstance = 0;

MenuStartAdmin* MenuStartAdmin::instance() {
    if (!m_pInstance) {
        m_pInstance = new MenuStartAdmin();
    }
    return m_pInstance;
}

MenuStartAdmin::MenuStartAdmin() : Panel("Administrator: Select an Action") {
    //Create buttons
    
    Button *pButton = new Button(this, "View an Application ", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 7, 10, 1);
    pButton->setEventHandler(handleViewPressed);
    this->add(pButton);
    
    pButton = new Button(this, "View Summary of Pending Applications",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleSummaryPressed);
    pButton->setUsrPtr(new ApplicationStatus(STATUS_PENDING));
    this->add(pButton);
    
    pButton = new Button(this, "View Summary of Assigned Applicants ",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleSummaryPressed);
    pButton->setUsrPtr(new ApplicationStatus(STATUS_ACCEPTED));
    this->add(pButton);
    
    /*pButton = new Button(this, "View and Edit a TA Evaluation ",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 5, 1);
    pButton->setEnabled(false);
    this->add(pButton);*/
    
    pButton = new Button(this, "Assign Successful Applicant ",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 5, 1);
    pButton->setEventHandler(handleAssignPressed);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 4, 1);
    pButton->setEventHandler(handleBackPressed);
    this->add(pButton);
}

MenuStartAdmin::~MenuStartAdmin() {
}

//Event Handlers

void MenuStartAdmin::handleBackPressed(Button *pButton) {
    MenuStartAdmin::instance()->hide();
}

void MenuStartAdmin::handleViewPressed(Button *pButton) {
    MenuStartAdmin::instance()->erase();
    MenuViewApplication *pMnu = new MenuViewApplication();
    pMnu->show();
    delete pMnu;
}

void MenuStartAdmin::handleSummaryPressed(Button *pButton) {
    MenuStartAdmin::instance()->erase();
    
    //Get a course selection from user
    
    //Get course list
    const std::string* courses;
    int count;
    Database::instance()->getCourses(&courses, count);
    //Show dialog
    DialogListSelector *pCourseSelector = new DialogListSelector("View Summary: Select a Course", courses, count, true);
    if (pCourseSelector->show() != STATE_SUCCESS) {
        delete pCourseSelector;
        MenuStartAdmin::instance()->draw();
        return;
    }
    std::string strCourse = pCourseSelector->getSelectedValue();
    if (strCourse.compare("all") == 0) {
        strCourse = "All Courses";
    }
    delete pCourseSelector;
    
    //Show list of applications
    if (pButton->getUsrPtr() == 0) {
        exit(-1);
    }
    ApplicationStatus* appStatus = (ApplicationStatus*)(pButton->getUsrPtr());
    MenuViewSummary *pViewSummary = new MenuViewSummary(strCourse + ": Pending Applications", strCourse, *appStatus);
    pViewSummary->show();
    delete pViewSummary;
    MenuStartAdmin::instance()->draw();
}

void MenuStartAdmin::handleAssignPressed(Button *pButton) {
    MenuStartAdmin::instance()->erase();
    
    //Get a course selection from user
    
    //Get course list
    const std::string* courses;
    int count;
    Database::instance()->getCourses(&courses, count);
    //Show dialog
    DialogListSelector *pCourseSelector = new DialogListSelector("Assign TA: Select a Course", courses, count);
    if (pCourseSelector->show() != STATE_SUCCESS) {
        delete pCourseSelector;
        MenuStartAdmin::instance()->draw();
        return;
    }
    std::string strCourse = pCourseSelector->getSelectedValue();
    delete pCourseSelector;
    
    //Show list of applications
    
    Queue<TaApplication*>* pApplications = Database::instance()->getApplications(strCourse);
    if (pApplications == 0 || pApplications->isEmpty()) {
        DialogYesNo* pMsg = new DialogYesNo("No applicants available for " + strCourse, DIALOG_MESSAGE);
        pMsg->show();
        delete pMsg;
        MenuStartAdmin::instance()->draw();
        return;
    }
    //Create str[] from queue for listDialog
    std::string* pStrApplications = new std::string[pApplications->getSize()];
    Node<TaApplication*>* pCur = pApplications->front();
    int i = 0;
    while (pCur != 0) {
        if (pCur->value->getStatus() == STATUS_PENDING) {
            pStrApplications[i] =  pCur->value->getStudentID();
            i++;
        }
        pCur = pCur->m_pNext;
    }
    if (i == 0) {
        //No applicants pending
        DialogYesNo* pMsg = new DialogYesNo("No applicants available for " + strCourse, DIALOG_MESSAGE);
        pMsg->show();
        delete pMsg;
        MenuStartAdmin::instance()->draw();
        return;
    }
    //Show the dialog
    DialogListSelector *pDia = new DialogListSelector("Assign TA: Select an Applicant", pStrApplications, i);
    if (pDia->show() != STATE_SUCCESS) {
        delete pDia;
        delete [] pStrApplications;
        MenuStartAdmin::instance()->draw();
        return;
    }
    std::string strApplication = pDia->getSelectedValue();
    delete [] pStrApplications;
    delete pDia;
    
    //Get the selected application
    
    TaApplication* pApp = 0;
    pCur = pApplications->front();
    while (pCur != 0) {
        if (strApplication.find(pCur->value->getStudentID()) != std::string::npos) {
            pApp = pCur->value;
        }
        pCur = pCur->m_pNext;
    }
    if (pApp == 0) {
        MenuStartAdmin::instance()->draw();
        return;
    }
    Student *pStu = Database::instance()->getStudent(pApp->getStudentID());
    if (pStu == 0) {
        exit(-1);
    }
    if (pStu->getApplications() != 0 && pStu->getApplications()->getSize() > 1) {
        pCur = pStu->getApplications()->front();
        while (pCur != 0) {
            if (pCur->value != pApp) {
                pCur->value->setStatus(STATUS_CLOSED);
                pCur->value->saveToFile();
            }
            pCur = pCur->m_pNext;
        }
    }
    pApp->setStatus(STATUS_ACCEPTED);
    pApp->saveToFile();
    DialogYesNo* pMsg = new DialogYesNo("Student " + pApp->getStudentID() + " has been assigned as TA.", DIALOG_MESSAGE);
    pMsg->show();
    delete pMsg;
    MenuStartAdmin::instance()->draw();
}
