#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
    class TerminalWindow
    {
    protected:
        WINDOW* m_window;
        int     m_lines,m_cols;
        float   m_begin_x, m_begin_y;
        chtype  m_hor_ch;
        chtype  m_ver_ch;
        bool    m_boxed;
        int     m_cursor_x, m_cursor_y;
    public:
        WINDOW* operator*();
        int refresh();
        int box(chtype verch, chtype horch);
        int mvwprintw(int y, int x, const std::string& text);
        int mvwprintw(int y, int x, const std::string&& text);
        int print(const std::string& text);
        int print(const std::string&& text);
        int move(float y, float x);
        int move();
        int erase();
        int rebox();
        int move_cursor(int y, int x);
        int cols();
        int lines();
        int delwin();
        virtual int resize() = 0;
    protected:
        TerminalWindow(const TerminalWindow& other) = default;
        TerminalWindow& operator=(const TerminalWindow& other) = default;
        TerminalWindow(TerminalWindow&& other) = default;
        TerminalWindow& operator=(TerminalWindow&& other) = default;

        TerminalWindow(int lines, int cols, float begin_y,
                float begin_x);
        TerminalWindow();
        void reset(int lines, int cols, float begin_y,
                   float begin_x);
    };
}
#endif
