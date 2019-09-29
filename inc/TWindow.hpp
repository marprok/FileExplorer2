#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
    class TWindow
    {
    private:
        WINDOW*                m_window;
        float                  m_per_cols, m_per_lines;
        float                  m_begin_x, m_begin_y;
        int                    m_parent_cols, m_parent_lines;
        chtype                 m_hor_ch;
        chtype                 m_ver_ch;
        bool                   m_boxed;
        int                    m_cursor_x, m_cursor_y;
    public:
        TWindow(float per_lines, float per_cols, float begin_y,
                float begin_x, int parent_lines, int parent_cols);
        WINDOW* operator*();
        int refresh();
        int box(chtype verch, chtype horch);
        int mvwprintw(int y, int x, const std::string& text);
        int mvwprintw(int y, int x, const std::string&& text);
        int move(float y, float x);
        int move();
        int resize(int parentlines, int parentcols);
        int erase();
        int rebox();
    };
}
#endif
