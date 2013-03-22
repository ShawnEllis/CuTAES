#include "MenuViewSummary.h"
#include <fstream>
#include <iostream>

#include "CuTAES.h"
#include "Label.h"
#include "Table.h"
#include "DialogYesNo.h"
#include "WindowUtil.h"

#include "StringUtil.h"
#include "Database.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"
#include "UndergradStudent.h"
#include "GradStudent.h"

#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

static std::string UNDERGRAD_LABELS[] = {"Student ID", "First", "Last", "Email", "Major", "Year Standing", "CGPA", "Major GPA"};
static int UNDERGRAD_COL_WIDTHS[]     = {10, 16, 16, 32, 32, 13, 4, 9};
static std::string GRAD_LABELS[]      = {"Student ID", "First", "Last", "Email", "Research Area", "Supervisor", "Program"};
static int GRAD_COL_WIDTHS[]          = {10, 16, 16, 32, 32, 20, 7};

MenuViewSummary::MenuViewSummary(const std::string& title, const std::string& course, ApplicationStatus appStatus) : Panel(title, 143, 1000), m_course(course) {
    setReturnState(STATE_ERROR);
    m_applicationType = appStatus;
    
    m_panelY = getY();

    int y = 3;
    if (course.compare("All Courses") == 0) {
        const std::string* courses;
        int count;
        Database::instance()->getCourses(&courses, count);
        for (int i = 0; i < count; i++) {
            add(new Label(this, courses[i], getWidth() / 2 - 4, y + 1));
            y += 3;
            y = createTablesForCourse(courses[i], y);
        }
    } else {
        y = createTablesForCourse(course, y);
    }
    setHeight(y + 1);
    drawComponents();
    
    int termWidth, termHeight;
    getmaxyx(stdscr, termHeight, termWidth);
    add(new Label(this, "Up/Down: Scroll", 0, termHeight - 2, true));
    add(new Label(this, "Enter: Back", 0, termHeight - 1, true));
    add(new Label(this, "Save Summary: F5", termWidth - 16, termHeight - 1, true));
}

MenuViewSummary::~MenuViewSummary() {
    //TODO: free mem
}

int MenuViewSummary::createTablesForCourse(const std::string &course, int y) {
    //Get applications
    Queue<UndergradStudent*>* pUndergrads = 0;
    Queue<GradStudent*>* pGrads = 0;
    Queue<TaApplication*>* pApplications = Database::instance()->getApplications(course, m_applicationType);
    getApplicantsByType(pApplications, &pUndergrads, &pGrads);
    delete pApplications;
    //Create tables
    Table *pTable = 0;
    if (pUndergrads != 0) {
        Student** pSortedList = sortByGPA(pUndergrads);
        pTable = createTable(pSortedList, pUndergrads->getSize(), y, true);
        delete [] pSortedList;
        if (pTable) {
            y = pTable->getHeight() + pTable->getY();
        }
        delete pUndergrads;
    }
    if (pGrads != 0) {
        Student** pSortedList = sortByResearchArea(pGrads);
        pTable = createTable(pSortedList, pGrads->getSize(), y, false);
        delete [] pSortedList;
        if (pTable) {
            y = pTable->getHeight() + pTable->getY();
        }
        delete pGrads;
    }
    return y;
}

/*
 * Creates a table for the given applications.
 * @param y: initial yPos of the table
 * @return: A list of applications that were not added, because the table went off screen TODO: this
 */
Table* MenuViewSummary::createTable(Student **pStudents, int numStudents, int y, bool isUndergrad) {
    if (pStudents == 0 || numStudents == 0) {
        return  0;
    }
    
    //Create table
    Table* pTable;
    if (isUndergrad) {
        pTable = new Table(this, 1, y, numStudents + 4, 8, UNDERGRAD_COL_WIDTHS, UNDERGRAD_LABELS, false);
    } else {
        pTable = new Table(this, 1, y, numStudents + 4, 7, GRAD_COL_WIDTHS, GRAD_LABELS, false);
    }
    
    std::string *data = 0;
    Student *pCurStudent = 0;
    for (int i = 0; i < numStudents; i++) {
        pCurStudent = pStudents[i];
        if (pCurStudent != 0) {
            data = isUndergrad ? new std::string[8] : new std::string[7];
            data[0] = pCurStudent->getStudentID();
            data[1] = pCurStudent->getFirstName();
            data[2] = pCurStudent->getLastName();
            data[3] = pCurStudent->getEmail();
            if (pCurStudent->getType() == TYPE_UNDERGRAD) {
                UndergradStudent* pTmp = (UndergradStudent*)pCurStudent;
                data[4] = pTmp->getMajor();
                data[5] = StringUtil::itos(pTmp->getYearStanding());
                data[6] = StringUtil::ftos(pTmp->getCGPA());
                data[7] = StringUtil::ftos(pTmp->getMajorGPA());
            } else if (pCurStudent->getType() == TYPE_GRAD) {
                GradStudent* pTmp = (GradStudent*)pCurStudent;
                data[4] = pTmp->getResearchArea();
                data[5] = pTmp->getSupervisor();
                data[6] = pTmp->getProgram();
            }
            pTable->addRow(data);
        }
    }
    
    add(pTable);
    
    return pTable;
}

/*
 * Returns applicants sorted into two lists by type.
 * You must free r_pUndergrads and r_pGrads.
 */
void MenuViewSummary::getApplicantsByType(Queue<TaApplication *>* pQueue, Queue<UndergradStudent*>** r_pUndergrads, Queue<GradStudent*>** r_pGrads) {
    if (pQueue == 0 || r_pUndergrads == 0 || *r_pUndergrads != 0 || r_pGrads == 0 || *r_pGrads != 0) {
        return;
    }
    Queue<UndergradStudent*>* pUndergrads = new Queue<UndergradStudent*>();
    Queue<GradStudent*>* pGrads = new Queue<GradStudent*>();
    
    Node <TaApplication*>* pCur = pQueue->front();
    Student* pStudent = 0;
    while (pCur != 0) {
        if (pCur->value != 0) {
            pStudent = Database::instance()->getStudent(pCur->value->getStudentID());
            if (pStudent != 0) {
                if (pStudent->getType() == TYPE_UNDERGRAD) {
                    pUndergrads->pushBack((UndergradStudent*)pStudent);
                } else {
                    pGrads->pushBack((GradStudent*)pStudent);
                }
            }
        }
        pCur = pCur->m_pNext;
    }
    
    *r_pUndergrads = pUndergrads;
    *r_pGrads = pGrads;
}



/*
 *  Sorts a queue of undergrads by GPA, returns a sorted array of undergrads
 */
Student** MenuViewSummary::sortByGPA(Queue<UndergradStudent*>* pQueue) {
    if (pQueue == 0) {
        return 0;
    }
    Student** pSorted = new Student*[pQueue->getSize()];
    UndergradStudent *pLastAdded = 0, *pBestSoFar = 0;
    Node<UndergradStudent*>* pCur = 0;
    for (int i = 0; i < pQueue->getSize(); i++) {
        //Find best
        pCur = pQueue->front();
        while (pCur != 0) {
            if (pLastAdded != 0 &&
                (pCur->value->getStudentID() == pLastAdded->getStudentID() ||
                 pCur->value->getMajorGPA() > pLastAdded->getMajorGPA())) {
                    //We have added this node already, skip
                    pCur = pCur->m_pNext;
                    continue;
            }
            if (pBestSoFar == 0 || pCur->value->getMajorGPA() > pBestSoFar->getMajorGPA()) {
                pBestSoFar = pCur->value;
            }
            pCur = pCur->m_pNext;
        }
        pSorted[i] = pBestSoFar;
        pLastAdded = pBestSoFar;
        pBestSoFar = 0;
    }
    return pSorted;
}

Student** MenuViewSummary::sortByResearchArea(Queue<GradStudent *>* pQueue) {
    if (pQueue == 0) {
        return 0;
    }
    Student **pSorted = new Student*[pQueue->getSize()];
    GradStudent *pLastAdded = 0, *pBestSoFar = 0;
    Node<GradStudent*>* pCur = 0;
    for (int i = 0; i < pQueue->getSize(); i++) {
        //Find next student alphabetically
        pCur = pQueue->front();
        while (pCur != 0) {
            if (pLastAdded != 0 &&
                (pCur->value->getStudentID() == pLastAdded->getStudentID() ||
                 pCur->value->getResearchArea().compare(pLastAdded->getResearchArea()) < 0)) {
                    //We have added this node already, skip
                    pCur = pCur->m_pNext;
                    continue;
            }
            if (pBestSoFar == 0 || pCur->value->getResearchArea().compare(pBestSoFar->getResearchArea()) < 0) {
                pBestSoFar = pCur->value;
            }
            pCur = pCur->m_pNext;
        }
        pSorted[i] = pBestSoFar;
        pLastAdded = pBestSoFar;
        pBestSoFar = 0;
    }
    return pSorted;
}

bool MenuViewSummary::handleKeyPress(int key) {
    if (key == CuTAES::KEY_ENT) {
        hide();
        return true;
    } else if (key == KEY_F(5)) {
        if (m_applicationType == STATUS_CLOSED) {
            return false;
        }
        //Save summary
        std::string filename = CuTAES::instance()->getDataDirectory() + "Summary_" + m_course
        + (m_applicationType == STATUS_PENDING ? "_Pending.txt" : "_Assigned.txt");
        char* scrData = new char[getWidth() * getHeight() + 256];
        mvinstr(0, 0, scrData);
        std::ofstream file;
        file.open(filename.data());
#ifdef DEBUG
        dout << "Saving summary for" << m_course << " to " << filename << std::endl;
#endif //DEBUG
        if (file.is_open()) {
            file << "--Summary of "
                << (m_applicationType == STATUS_PENDING ? "Pending" : "Assigned")
                << " Applications for " << m_course << "--";
            if (m_course.compare("All Courses") == 0) {
                const std::string* courses;
                int count;
                Database::instance()->getCourses(&courses, count);
                for (int i = 0; i < count; i++) {
                    file << std::endl << std::endl << courses[i] << std::endl << std::endl;
                    saveCourseData(courses[i], &file);
                }
            } else {
                file << std::endl << std::endl;
                saveCourseData(m_course, &file);
            }
            
            DialogYesNo *pDia = new DialogYesNo("Summary saved successfully.", DIALOG_MESSAGE);
            pDia->show();
            delete pDia;
            file.close();
        }
    } else if (key == KEY_UP) {
        if (getScrollY() > 0) {
            scrollY(-1);
        }
    } else if (key == KEY_DOWN) {
        int termWidth, termHeight;
        getmaxyx(stdscr, termHeight, termWidth);
        if (getScrollY() < getHeight() - termHeight) {
            scrollY(1);
        }
    }
    return false;
}

void MenuViewSummary::saveCourseData(const std::string &course, std::ofstream *file) {
    Queue<UndergradStudent*>* pUndergrads = 0;
    Queue<GradStudent*>* pGrads = 0;
    getApplicantsByType(Database::instance()->getApplications(course, m_applicationType), &pUndergrads, &pGrads);
    if (pUndergrads != 0) {
        *file << '|' << "Student ID" << '|';
        *file << StringUtil::pad("First Name", 16) << '|';
        *file << StringUtil::pad("Last Name", 16) << '|';
        *file << StringUtil::pad("Email", 32) << '|';
        *file << StringUtil::pad("Major", 32) << '|';
        *file << "Year Standing" << '|';
        *file << "CGPA" << '|';
        *file << "Major GPA" << '|'  << std::endl;
        std::string* data = 0;
        Node<UndergradStudent*>* pCur = pUndergrads->front();
        while (pCur != 0) {
            pCur->value->getData(&data);
            *file << '|' <<  StringUtil::pad(data[0], 10) << '|';
            *file << StringUtil::pad(data[1], 16) << '|';
            *file << StringUtil::pad(data[2], 16) << '|';
            *file << StringUtil::pad(data[3], 32) << '|';
            *file << StringUtil::pad(data[4], 32) << '|';
            *file << StringUtil::pad(data[5], 13) << '|';
            *file << StringUtil::pad(data[6], 4) << '|';
            *file << StringUtil::pad(data[7], 9) << '|' << std::endl;
            data = 0;
            pCur = pCur->m_pNext;
        }
    }
    if (pGrads != 0) {
        *file << std::endl << '|' << "Student ID" << '|';
        *file << StringUtil::pad("First Name", 16) << '|';
        *file << StringUtil::pad("Last Name", 16) << '|';
        *file << StringUtil::pad("Email", 32) << '|';
        *file << StringUtil::pad("Research Area", 32) << '|';
        *file << StringUtil::pad("Supervisor", 20) << '|';
        *file << "Program" << '|' << std::endl;
        std::string* data = 0;
        Node<GradStudent*>* pCur = pGrads->front();
        while (pCur != 0) {
            pCur->value->getData(&data);
            *file << '|' <<  StringUtil::pad(data[0], 10) << '|';
            *file << StringUtil::pad(data[1], 16) << '|';
            *file << StringUtil::pad(data[2], 16) << '|';
            *file << StringUtil::pad(data[3], 32) << '|';
            *file << StringUtil::pad(data[6], 32) << '|';
            *file << StringUtil::pad(data[4], 20) << '|';
            *file << StringUtil::pad(data[5], 7) << '|' << std::endl;
            data = 0;
            pCur = pCur->m_pNext;
        }
    }
}

void MenuViewSummary::savePending() {
    
}

void MenuViewSummary::saveAssigned() {
    
}