#include "MenuStartStudent.h"
#include <stdlib.h>

#include "Button.h"
#include "DialogForm.h"
#include "DialogListSelector.h"
#include "DialogYesNo.h"
#include "MenuCreateApplication.h"

#include "Database.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"
#include "UndergradStudent.h"
#include "GradStudent.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

MenuStartStudent::MenuStartStudent(bool undergrad) : Panel("Student: Select an Action"), m_undergrad(undergrad) {
    //Create buttons
    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 6, 5);
    pButton->setEventHandler(handleCreatePressed);
    pButton->setUsrPtr(this);
    this->add(pButton);

    pButton = new Button(this, "Edit an Application", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEventHandler(handleEditPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEventHandler(handleBackPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
}

MenuStartStudent::~MenuStartStudent() {
}

//Event Handlers

void MenuStartStudent::handleBackPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        MenuStartStudent *pMenu = ((MenuStartStudent*)usrPtr);
        pMenu->hide();
    }
}

void MenuStartStudent::handleEditPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    //Get a pointer to the MenuStartStudent instance
    void* usrPtr = pButton->getUsrPtr();
    MenuStartStudent *pMenu;
    if (usrPtr == 0) {
        return;
    }
    pMenu = ((MenuStartStudent*)usrPtr);
    pMenu->erase();
    
    //Get the user's student number
    DialogForm *pForm = new DialogForm("Enter Student ID", 1);
    pForm->addField("Student ID: ",
                    CuTAES::instance()->getActiveUser() == 0 ? "" : CuTAES::instance()->getActiveUser()->getStudentID(),
                    1, 32, FIELDTYPE_INT);
    std::string *data = 0;
    if (pForm->show() != STATE_SUCCESS || !pForm->getFormData(&data)) {
        pMenu->draw();
        delete pForm;
        return;
    }
    delete pForm;
    std::string stuNum;
    stuNum = data[0];
    delete [] data;

    Student* pStudent = Database::instance()->getStudent(stuNum);
    if (pStudent == 0) {
        DialogYesNo* pDia = new DialogYesNo("Student " + stuNum + " not found.", DIALOG_MESSAGE);
        pDia->show();
        delete pDia;
        pMenu->draw();
        return;
    }
    
    //Check that student is correct type
    if (pStudent->getType() == TYPE_GRAD && pMenu->m_undergrad) {
        DialogYesNo* pDia = new DialogYesNo("Student " + stuNum + " is not an undergrad.", DIALOG_MESSAGE);
        pDia->show();
        delete pDia;
        pMenu->draw();
        return;
    } else if (pStudent->getType() == TYPE_UNDERGRAD && !pMenu->m_undergrad) {
        DialogYesNo* pDia = new DialogYesNo("Student " + stuNum + " is not a graduate.", DIALOG_MESSAGE);
        pDia->show();
        delete pDia;
        pMenu->draw();
        return;
    }
    
    
    //Show applications for student

    Queue<TaApplication*>* pApplications = pStudent->getApplications();
    if (pApplications == 0 || pApplications->isEmpty()) {
        DialogYesNo* pDia = new DialogYesNo("No Applications for Student " + stuNum, DIALOG_MESSAGE);
        pDia->show();
        delete pDia;
        pMenu->draw();
        return;
    }
    //Create str[] from queue for dialog
    std::string* pStrApplications = new std::string[pApplications->getSize()];
    Node<TaApplication*>* pCur = pApplications->front();
    int i = 0;
    while (pCur != 0) {
        if (pCur->value->getStatus() == STATUS_PENDING) {
            pStrApplications[i] =  '[' + pCur->value->getStrStatus() + "] " + pCur->value->getCourse();
        } else {
            pStrApplications[i] =  "~[" + pCur->value->getStrStatus() + "] " + pCur->value->getCourse();
        }
        i++;
        pCur = pCur->m_pNext;
    }
    //Show the dialog
    DialogListSelector *pDia = new DialogListSelector("Edit Application: Select an Application", pStrApplications, i);
    if (pDia->show() != STATE_SUCCESS) {
        delete pDia;
        delete [] pStrApplications;
        pMenu->draw();
        return;
    }
    std::string strApplication = pDia->getSelectedValue();
    delete [] pStrApplications;
    delete pDia;
    
    //Get the selected application by course
    
    TaApplication* pApp = 0;
    pCur = pApplications->front();
    while (pCur != 0) {
        if (strApplication.find(pCur->value->getCourse()) != std::string::npos) {
            pApp = pCur->value;
        }
        pCur = pCur->m_pNext;
    }
    
    if (pApp == 0) {
        pMenu->draw();
        return;
    }
    
    MenuCreateApplication* pMenuEdit = new MenuCreateApplication(pApp, Database::instance()->getStudent(stuNum)->getType() == TYPE_GRAD);
    TaApplication *pEditedApp = 0;
    if (pMenuEdit->show() != STATE_SUCCESS || !pMenuEdit->getData(&pEditedApp)) {
        delete pMenuEdit;
        pMenu->draw();
        return;
    }
    Database::instance()->replaceApplication(pApp, pEditedApp);
    
    delete pMenuEdit;
    pMenu->draw();
}

void MenuStartStudent::handleCreatePressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    //Get a pointer to the MenuStartStudent instance
    void* usrPtr = pButton->getUsrPtr();
    MenuStartStudent *pMenu;
    if (usrPtr != 0) {
        pMenu = ((MenuStartStudent*)usrPtr);
    }
    
    pMenu->erase();
    
    //Get the active student
    Student *pActiveStudent = CuTAES::instance()->getActiveUser();
    
    bool shouldExit = false;
    while (!shouldExit) {
        
        //Get a course selection from user
        
        //Get course list
        const std::string* courses;
        int count;
        Database::instance()->getCourses(&courses, count);
        //Show dialog
        DialogListSelector *pCourseSelector = new DialogListSelector("Create Application: Select a Course", courses, count);
        if (pCourseSelector->show() != STATE_SUCCESS) {
            delete pCourseSelector;
            break;
        }
        std::string strCourse = pCourseSelector->getSelectedValue();
        delete pCourseSelector;
        
        //Get student info
        
        Student* pNewStu;
        if (pMenu->isUndergrad()) {
            if (pActiveStudent != 0 && pActiveStudent->getType() == TYPE_UNDERGRAD) {
                pNewStu = pMenu->showUndergradDialog((UndergradStudent*)pActiveStudent);
            } else {
                pNewStu = pMenu->showUndergradDialog(0);
            }
        } else {
            if (pActiveStudent != 0 && pActiveStudent->getType() == TYPE_GRAD) {
                pNewStu = pMenu->showGradDialog((GradStudent*)pActiveStudent);
            } else {
                pNewStu = pMenu->showGradDialog(0);
            }
        }

        
        if (pNewStu == 0) {
            break;
        }
        //If no active student or fields have been modified
        if (pActiveStudent == 0 || !pActiveStudent->equals(pNewStu)) {
            //Find a student with the given id in the database
            Student *pSavedStudent = Database::instance()->getStudent(pNewStu->getStudentID());
            if (pSavedStudent != 0) {
                //Student exists with that ID. Edit it?
                DialogYesNo *pDialog = new DialogYesNo("Update student " + pNewStu->getStudentID() + "?", DIALOG_YESNO);
                StateType state = pDialog->show();
                delete pDialog;
                if (state != STATE_SUCCESS) {
                    break;                        
                }
                pNewStu->setApplications(pSavedStudent->getApplications());
                Database::instance()->replaceStudent(pSavedStudent, pNewStu);
                CuTAES::instance()->setActiveUser(pNewStu);
                pActiveStudent = pNewStu;
            } else {
                Database::instance()->addStudent(pNewStu);
                CuTAES::instance()->setActiveUser(pNewStu);
                pActiveStudent = pNewStu;
            }
        }
        
        //Ensure the student hasn't applied to this course before
        
        if (pActiveStudent->hasAppliedForCourse(strCourse)) {
            DialogYesNo* pDia = new DialogYesNo("You have already applied for this course.", DIALOG_MESSAGE);
            pDia->show();
            delete pDia;
            break;
        }
        
        //Get application info
        
        MenuCreateApplication *pCreateApplication = new MenuCreateApplication(strCourse, pActiveStudent->getStudentID(), !pMenu->isUndergrad());
        if (pCreateApplication->show() != STATE_SUCCESS) {
            delete pCreateApplication;
            break;
        }
        
        TaApplication *pApplication = 0;
        if (pCreateApplication->getData(&pApplication)) {
            Database::instance()->addApplication(pApplication);
            pActiveStudent->addApplication(pApplication);
        }
        
        delete pCreateApplication;
        
        DialogYesNo *pDia = new DialogYesNo("Apply for another course?");
        if (pDia->show() != STATE_SUCCESS) {
            shouldExit = true;
        }
        delete pDia;
    }
    pMenu->draw();
}

Student* MenuStartStudent::showUndergradDialog(UndergradStudent *pActiveStudent) {
    //Create student info dialog
    DialogForm *pForm = new DialogForm("Enter Student Info", 8);    
    pForm->addField("First Name:   ", pActiveStudent == 0 ? "" : pActiveStudent->getFirstName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", pActiveStudent == 0 ? "" : pActiveStudent->getLastName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", pActiveStudent == 0 ? "" : pActiveStudent->getStudentID(), 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", pActiveStudent == 0 ? "" : pActiveStudent->getEmail(), 1, 32);
    pForm->addField("Major:        ", pActiveStudent == 0 ? "" : pActiveStudent->getMajor(), 1, 32);
    pForm->addField("Year Standing:", pActiveStudent == 0 ? "" : StringUtil::itos(pActiveStudent->getYearStanding()), 1, 2, FIELDTYPE_INT);
    int range[] = {0, 1, 12};// TODO: Investigate why float validation is broken on OS X
    pForm->addField("CGPA:", pActiveStudent == 0 ? "" : StringUtil::ftos(pActiveStudent->getCGPA()), 1, 4, 20, 10, FIELDTYPE_FLOAT, range, 3);
    pForm->addField("Major GPA:", pActiveStudent == 0 ? "" : StringUtil::ftos(pActiveStudent->getMajorGPA()), 1, 4, 32, 10, FIELDTYPE_FLOAT, range, 3);
    
    //Show student dialog
    if (pForm->show() != STATE_SUCCESS) {
        return 0;
    }
    
    //Get result
    std::string *data = 0;
    bool isDataValid = pForm->getFormData(&data);
    delete pForm;
    if (isDataValid) {
        UndergradStudent *pStu = new UndergradStudent(data);
        delete [] data;
        return pStu;
    }
    return 0;
}

Student* MenuStartStudent::showGradDialog(GradStudent *pActiveStudent) {
    //Create student info dialog
    DialogForm *pForm = new DialogForm("Enter Student Info", 6);    
    pForm->addField("First Name:   ", pActiveStudent == 0 ? "" : pActiveStudent->getFirstName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", pActiveStudent == 0 ? "" : pActiveStudent->getLastName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", pActiveStudent == 0 ? "" : pActiveStudent->getStudentID(), 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", pActiveStudent == 0 ? "" : pActiveStudent->getEmail(), 1, 32);
    pForm->addField("Supervisor:   ", pActiveStudent == 0 ? "" : pActiveStudent->getSupervisor(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Program:      ", pActiveStudent == 0 ? "" : pActiveStudent->getProgram(), 1, 4, FIELDTYPE_ALPHA);
    
    //Show student dialog
    if (pForm->show() != STATE_SUCCESS) {
        return 0;
    }
    
    //Get result
    std::string *pFormData = 0;
    bool isDataValid = pForm->getFormData(&pFormData);
    delete pForm;
    if (isDataValid) {
        const std::string* researchAreas;
        int count;
        Database::instance()->getResearchAreas(&researchAreas, count);
        //Show dialog
        DialogListSelector *pDia = new DialogListSelector("Create Application: Choose Research Area", researchAreas, count);
        if (pDia->show() != STATE_SUCCESS) {
            delete pDia;
            return 0;
        }
        //Copy data into larger array
        std::string *pData = new std::string[7];
        for (int i = 0; i < 6; i++) {
            pData[i] = pFormData[i];
        }
        delete [] pFormData;
        pData[6] = pDia->getSelectedValue();
        StringUtil::trim(pData[6]);
        delete pDia;
        
        GradStudent* pStu = new GradStudent(pData);
        delete [] pData;
        return pStu;
    }
    return 0;
}
