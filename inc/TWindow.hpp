#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
    class TWindow
    {
    protected:
        WINDOW* m_window;
        int    m_lines,m_cols;
        int   m_begin_x, m_begin_y;
        chtype  m_hor_ch;
        chtype  m_ver_ch;
        bool    m_boxed;
        int     m_cursor_x, m_cursor_y;
    public:
        TWindow(int lines, int cols, int begin_y,
                int begin_x);
        TWindow();
        WINDOW* operator*();
        int refresh();
        int box(chtype verch, chtype horch);
        int mvwprintw(int y, int x, const std::string& text);
        int mvwprintw(int y, int x, const std::string&& text);
        int print(const std::string& text);
        int print(const std::string&& text);
        int move(int y, int x);
        int erase();
        int rebox();
        int move_cursor(int y, int x);
        int cols();
        int lines();
        int delwin();
    protected:
        void reset(int lines, int cols, int begin_y,
                           int begin_x);
    };
}
#endif
