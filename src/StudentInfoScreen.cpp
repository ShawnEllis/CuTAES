#include "StudentInfoScreen.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include <string>
#include <stdlib.h>
#include "Student.h"
#include "StringUtil.h"

using namespace std;

StudentInfoScreen::StudentInfoScreen() : Panel("") {
    //Create fields
    infoFields[0] = new_field(1, 32, 1, 12, 0, 0);
    set_field_type(infoFields[0], TYPE_ALPHA, 1);
	infoFields[1] = new_field(1, 32, 3, 12, 0, 0);
    set_field_type(infoFields[1], TYPE_ALPHA, 1);
	infoFields[2] = new_field(1, 32, 5, 12, 0, 0);
    set_field_type(infoFields[2], TYPE_INTEGER, 0);
	infoFields[3] = new_field(1, 32, 7, 12, 0, 0);
    infoFields[4] = new_field(1, 32, 9, 12, 0, 0);
	infoFields[5] = new_field(1, 2, 11, 14, 0, 0);
    set_field_type(infoFields[5], TYPE_INTEGER, 0);
    infoFields[6] = new_field(1, 4, 11, 24, 0, 0);
    set_field_type(infoFields[6], TYPE_NUMERIC, 1, 1, 12);
	infoFields[7] = new_field(1, 4, 11, 40, 0, 0);
    set_field_type(infoFields[7], TYPE_NUMERIC, 1, 1, 12);
    infoFields[8] = 0;

    for (int i = 0; i < 8; i++) {
        set_field_back(infoFields[i], A_UNDERLINE);
    	field_opts_off(infoFields[i], O_AUTOSKIP);
    }

    int rows, cols;

    //Create form & window
    pForm = new_form(infoFields);
    scale_form(pForm, &rows, &cols);
    m_pWindow = newwin(rows + 5, cols + 4, 4, CuTAES::DEF_W / 2 - (cols + 4) / 2);

    set_form_win(pForm, m_pWindow);
    set_form_sub(pForm, derwin(m_pWindow, rows, cols, 2, 2));
    box(m_pWindow, 0, 0);

    post_form(pForm);

    //Write title and form labels
    mvwprintw(m_pWindow, 1, 1, "Student Information:");
    mvwprintw(m_pWindow, 3, 1, "First Name:");
	mvwprintw(m_pWindow, 5, 1, "Last Name:");
    mvwprintw(m_pWindow, 7, 1, "Student ID:");
	mvwprintw(m_pWindow, 9, 1, "Email:");
    mvwprintw(m_pWindow, 11, 1, "Major:");
	mvwprintw(m_pWindow, 13, 1, "Year Standing:");
    mvwprintw(m_pWindow, 13, 20, "CGPA:");
	mvwprintw(m_pWindow, 13, 31, "Major GPA:");
	mvwprintw(m_pWindow, 15, 1, "`/~: Cancel");
	mvwprintw(m_pWindow, 15, cols - 11, "ENTER: Accept");

    form_driver(pForm, REQ_END_LINE);

    curs_set(1);
}

StudentInfoScreen::~StudentInfoScreen() {
    curs_set(0);
    unpost_form(pForm);
    free_form(pForm);
    for (int i = 0; i < 8; i++) {
        free_field(infoFields[i]);
    }
    refresh();
}

void StudentInfoScreen::show() {
    //Show the window
    wrefresh(m_pWindow);
    waitForInput();
}

void StudentInfoScreen::waitForInput() {
    while (true) {
        int ch = getch();
        if (ch == KEY_UP) {
            form_driver(pForm, REQ_PREV_FIELD);
            form_driver(pForm, REQ_END_LINE);
            wrefresh(m_pWindow);
        } else if (ch == KEY_DOWN) {
            form_driver(pForm, REQ_NEXT_FIELD);
            form_driver(pForm, REQ_END_LINE);
            wrefresh(m_pWindow);
        } else if (ch == KEY_LEFT) {
            form_driver(pForm, REQ_LEFT_CHAR);
            wrefresh(m_pWindow);
        } else if (ch == KEY_RIGHT) {
            form_driver(pForm, REQ_RIGHT_CHAR); //TODO: Stop right arrow working as spacebar
            wrefresh(m_pWindow);
        } else if (ch == KEY_BACKSPACE) {
            form_driver(pForm, REQ_DEL_PREV);
            wrefresh(m_pWindow);
        } else if (ch == CuTAES::KEY_ENT) {
            //TODO: Check if form is full
            form_driver(pForm, REQ_NEXT_FIELD); //TODO: Find better way to validate cur field
            string fname = field_buffer(infoFields[0], 0);
            StringUtil::trimEnd(fname);
            string lname = field_buffer(infoFields[1], 0);
            StringUtil::trimEnd(lname);
            string num = field_buffer(infoFields[2], 0);
            StringUtil::trimEnd(num);
            string email = field_buffer(infoFields[3], 0);
            StringUtil::trimEnd(email);
            string major = field_buffer(infoFields[4], 0);
            StringUtil::trimEnd(major);
            int year = atoi(field_buffer(infoFields[5], 0));
            float cgpa = atof(field_buffer(infoFields[6], 0));
            float majGpa = atof(field_buffer(infoFields[7], 0));
            Student *stu = new Student(fname, lname, num, email, major, year, cgpa, majGpa);
            stu->saveToFile();
            break;
        } else if (ch == 96) {
            //Cancel
            break;
        } else {
            form_driver(pForm, ch);
            wrefresh(m_pWindow);
        }
    }
}
