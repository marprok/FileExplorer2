#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
    class TWindow
    {
    private:
        WINDOW              *m_window;
        int                 m_ncols, m_nlines;
        int                 m_begin_x, m_begin_y;
        chtype                 m_horch;
        chtype                 m_verch;
        bool                   m_boxed;
        int                    m_cursorx, m_cursory;
    public:
        TWindow(int nlines, int ncols, int begin_y, int begin_x);
        WINDOW* operator*();
        int refresh();
        int box(chtype verch, chtype horch);
        int mvwprintw(int y, int x, const std::string& text);
        int mvwprintw(int y, int x, const std::string&& text);
        int move(int y, int x);
        int resize(int nlines, int ncols);
        int erase();
        int rebox();
    };
}
#endif
