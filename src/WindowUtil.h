
#ifndef WINDOW_UTIL_H
#define WINDOW_UTIL_H

#include <curses.h>
#include <string>

class Button;
class Panel;

class WindowUtil {
public:
	static void createTitledWindow(WINDOW **pWindow, const std::string &title);

    static void drawHLine(WINDOW *pWindow, int x, int y, int l, char c='_');
    static void drawVLine(WINDOW *pWindow, int x, int y, int l, char c='|');

    static void drawRect(WINDOW *pWindow, int x, int y, int w, int h);
    static void fillRect(WINDOW *pWindow, int x, int y, int w, int h, char c='\'');
};

#endif //WINDOW_UTIL_H


