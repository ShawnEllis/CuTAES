//
//  MenuViewSummary.h
//  CuTAES
//
//  Created by Zack Dawson on 13-02-13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef MENU_VIEW_SUMMARY_H
#define MENU_VIEW_SUMMARY_H

#include "Panel.h"
#include "TaApplication.h"

template <class T>
class Queue;
class Table;
class UndergradStudent;
class GradStudent;

class MenuViewSummary : public Panel {
public:
    MenuViewSummary(const std::string& title, const std::string& course, ApplicationStatus appStatus = STATUS_PENDING);
    virtual ~MenuViewSummary();
    
    virtual bool handleKeyPress(int key);
    
private:
    int createTablesForCourse(const std::string& course, int y);
    void saveCourseData(const std::string& course, std::ofstream *file);
    
    Table* createTable(Student**, int count, int y, bool isUndergrad);
    
    void getApplicantsByType(Queue<TaApplication*>* pQueue, Queue<UndergradStudent*>**, Queue<GradStudent*>**);
    
    Student** sortByGPA(Queue<UndergradStudent*>* pQueue);
    Student** sortByResearchArea(Queue<GradStudent*>* pQueue);
    
    void savePending();
    void saveAssigned();
    
    int m_panelY;
    
    ApplicationStatus m_applicationType;
    
    const std::string& m_course;
};

#endif //MENU_VIEW_SUMMARY_H
