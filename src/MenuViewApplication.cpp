#include "MenuViewApplication.h"
#include <stdlib.h>

#include "Database.h"
#include "TaApplication.h"
#include "Student.h"
#include "Queue.h"

#include "Button.h"
#include "DialogForm.h"
#include "DialogYesNo.h"
#include "DialogListSelector.h"
#include "MenuCreateApplication.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

MenuViewApplication::MenuViewApplication() : Panel("View Application") {
    //Create buttons
    Button *pButton = new Button(this, "Search By Application ID", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 5, 2, 1);
    pButton->setEventHandler(handleSearchAppPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);

    pButton = new Button(this, "Search by Student ID", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleSearchStuPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);

    pButton = new Button(this, "Search by Name", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleSearchNamePressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleBackPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
}

MenuViewApplication::~MenuViewApplication() {
}

//Event Handlers

void MenuViewApplication::handleBackPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        MenuViewApplication *pMenu = ((MenuViewApplication*)usrPtr);
        pMenu->hide();
    }
}

void MenuViewApplication::handleSearchAppPressed(Button *pButton) {
    //Show Dialog Form w/ application num field
    DialogForm *pForm = new DialogForm("Enter Application ID", 1);
    pForm->addField("Application ID: ", "", 1, 32, FIELDTYPE_NONE);
    //Show student dialog
    if (pForm->show() == STATE_SUCCESS) {
        //Get result
        std::string *data = 0;
        bool isDataValid = pForm->getFormData(&data);
        delete pForm;
        if (isDataValid) {
            //Find application
            TaApplication* pApp = Database::instance()->findApplication(data[0]);
            if (pApp != 0) {
                //Show application
                MenuCreateApplication *pCreateApplication = new MenuCreateApplication(pApp,
                Database::instance()->getStudent(pApp->getStudentID())->getType() == TYPE_GRAD, false);
                pCreateApplication->show();
                delete pCreateApplication;
            } else {
                DialogYesNo* pDia = new DialogYesNo("Application not found.", DIALOG_MESSAGE);
                pDia->show();
                delete pDia;
            }
        }
    }
}

void MenuViewApplication::handleSearchStuPressed(Button *pButton) {
    //Show Dialog Form w/ student num field
    DialogForm *pForm = new DialogForm("Enter Student ID", 1);    
    pForm->addField("Student ID: ", "", 1, 32, FIELDTYPE_INT);
    //Show student dialog
    if (pForm->show() == STATE_SUCCESS) {
        //Get result
        std::string *data = 0;
        bool isDataValid = pForm->getFormData(&data);
        delete pForm;
        if (isDataValid) {
            //Find application
            Student* pStu = Database::instance()->getStudent(data[0]);
            if (pStu != 0) {
                Queue<TaApplication*>* pApplications = pStu->getApplications();
                if (pApplications != 0 && pApplications->getSize() > 0) {
                    //Create str[] from queue for listDialog
                    std::string* pStrApplications = new std::string[pApplications->getSize()];
                    Node<TaApplication*>* pCur = pApplications->front();
                    int i = 0;
                    while (pCur != 0) {
                        pStrApplications[i] = pCur->value->getApplicationID();
                        i++;
                        pCur = pCur->m_pNext;
                    }
                    //Show the dialog
                    DialogListSelector *pDia = new DialogListSelector("View Application", pStrApplications, i);
                    if (pDia->show() == STATE_SUCCESS) {
                        std::string strApplication = pDia->getSelectedValue();
                        
                        //Find the selected application
                        TaApplication* pApp = 0;
                        pCur = pApplications->front();
                        while (pCur != 0) {
                            if (strApplication.find(pCur->value->getApplicationID()) != std::string::npos) {
                                pApp = pCur->value;
                            }
                            pCur = pCur->m_pNext;
                        }
                        if (pApp != 0) {
                            //Show application
                            MenuCreateApplication *pCreateApplication = new MenuCreateApplication(pApp,
                                Database::instance()->getStudent(pApp->getStudentID())->getType() == TYPE_GRAD, false);
                            pCreateApplication->show();
                            delete pCreateApplication;
                        }
                    }
                    delete [] pStrApplications;
                    delete pDia;
                } else {
                    DialogYesNo* pDia = new DialogYesNo("No applications to display.", DIALOG_MESSAGE);
                    pDia->show();
                    delete pDia;
                }
            } else {
                DialogYesNo* pDia = new DialogYesNo("Student not found.", DIALOG_MESSAGE);
                pDia->show();
                delete pDia;
            }
        }
    }
}

void MenuViewApplication::handleSearchNamePressed(Button *pButton) {
    //Show Dialog Form w/ student num field
    DialogForm *pForm = new DialogForm("Enter Student Name (First/Last/Both)", 1);    
    pForm->addField("First Name: ", "", 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:  ", "", 1, 32, FIELDTYPE_ALPHA);
    //Show student dialog
    if (pForm->show() == STATE_SUCCESS) {
        //Get result
        std::string *data = 0;
        bool isDataValid = pForm->getFormData(&data);
        delete pForm;
        if (isDataValid) {
            //Find application
            
        }
    }
}