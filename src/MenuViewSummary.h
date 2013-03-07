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

template <class T>
class Queue;
class TaApplication;
class Table;
class UndergradStudent;
class GradStudent;

class MenuViewSummary : public Panel {
public:
    MenuViewSummary(const std::string& title, const std::string& course);
    virtual ~MenuViewSummary();
    
    virtual bool handleKeyPress(int key);
    
private:
    int createTablesForCourse(const std::string& course, int y);
    
    Table* createTable(Student**, int count, int y, bool isUndergrad);
    
    void getApplicantsByType(Queue<TaApplication*>* pQueue, Queue<UndergradStudent*>**, Queue<GradStudent*>**);
    
    Student** sortByGPA(Queue<UndergradStudent*>* pQueue);
    Student** sortByResearchArea(Queue<GradStudent*>* pQueue);
    
    int m_panelY;
    
    const std::string& m_course;
};

#endif //MENU_VIEW_SUMMARY_H
