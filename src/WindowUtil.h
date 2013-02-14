
#ifndef WINDOW_UTIL_H
#define WINDOW_UTIL_H

#include <curses.h>
#include <string>

class Button;
class Panel;

class WindowUtil {
public:
	static void createTitledWindow(WINDOW **pWindow, const std::string &title);

    static void drawHLine(WINDOW *pWindow, int x, int y, int l, int ch=ACS_HLINE);
    static void drawVLine(WINDOW *pWindow, int x, int y, int l, int ch=ACS_VLINE);

    static void drawRect(WINDOW *pWindow, int x, int y, int w, int h,
                         int t=ACS_HLINE,     int b=ACS_HLINE,
                         int l=ACS_VLINE,     int r=ACS_VLINE,
                         int tl=ACS_ULCORNER, int tr=ACS_URCORNER,
                         int bl=ACS_LLCORNER, int br=ACS_LRCORNER);
    
    static void fillRect(WINDOW *pWindow, int x, int y, int w, int h, int ch=ACS_CKBOARD);
};

#endif //WINDOW_UTIL_H


