#include "CourseInfoScreen.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include "CuTAES.h"
#include "Student.h"
#include "StringUtil.h"
#include "WindowUtil.h"

using namespace std;

CourseInfoScreen::CourseInfoScreen() : Panel("") {
    //Create fields
    infoFields[0] = new_field(5, 32, 3, 0, 0, 0);
    infoFields[1] = new_field(5, 32, 11, 0, 0, 0);
    infoFields[2] = new_field(5, 32, 3, 33, 0, 0);
    infoFields[3] = new_field(5, 32, 11, 33, 0, 0);
    infoFields[4] = 0;

    for (int i = 0; i < 4; i++) {
        set_field_back(infoFields[i], A_UNDERLINE);
    	field_opts_off(infoFields[i], O_AUTOSKIP);
    }

    int rows, cols;

    //Create form & window
    pForm = new_form(infoFields);
    scale_form(pForm, &rows, &cols);
    m_pWindow = newwin(rows + 5, cols + 4, 2, CuTAES::DEF_W / 2 - (cols + 4) / 2);

    set_form_win(pForm, m_pWindow);
    set_form_sub(pForm, derwin(m_pWindow, rows, cols, 2, 2));
    box(m_pWindow, 0, 0);

    post_form(pForm);

    //Write title and form labels
    mvwprintw(m_pWindow, 1, 2, "Course Information");

    mvwprintw(m_pWindow, 3, 2, "Related courses taken:");
    mvwprintw(m_pWindow, 4, 2, "(incl. term, year, final grade)");

    mvwprintw(m_pWindow, 3, 35, "Related courses TA'd:");
    mvwprintw(m_pWindow, 4, 35, "(incl. term, year, supervisor)");

    mvwprintw(m_pWindow, 11, 2, "Relevant work experience:");
    mvwprintw(m_pWindow, 12, 2, "(incl. duration and dates)");

    mvwprintw(m_pWindow, 12, 35, "List of faculty references:");

	mvwprintw(m_pWindow, 19, 1, "`/~: Cancel");
	mvwprintw(m_pWindow, 19, cols - 11, "ENTER: Accept");

    form_driver(pForm, REQ_END_LINE);

    curs_set(1);
}

CourseInfoScreen::~CourseInfoScreen() {
    curs_set(0);
    unpost_form(pForm);
    free_form(pForm);
    for (int i = 0; i < 5; i++) {
        free_field(infoFields[i]);
    }
    refresh();
}

void CourseInfoScreen::show() {
    //Show the window
    wrefresh(m_pWindow);
    waitForInput();
}

void CourseInfoScreen::waitForInput() {
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
            //Save application
            char buffer[50];
            sprintf(buffer, "application%ld.txt", time(NULL));
            ofstream file;
            string filename = CuTAES::instance()->getDataDirectory() + buffer;
            file.open(filename.data());
            if (file.is_open()) {
                file << "Pending\n\n";
                for (int i = 0; i < 4; i++) {
                    string str = field_buffer(infoFields[i], 0);
                    file << "Field " << i << "\n" << str << "\n\n\n";
                }
                file.close();
            }
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
