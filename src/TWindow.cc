#include "../inc/TWindow.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    TWindow::TWindow(float per_lines, float per_cols, float begin_y,
                     float begin_x, int parent_lines, int parent_cols)

        :m_window(nullptr),
         m_per_cols(per_cols),
         m_per_lines(per_lines),
         m_begin_x(begin_x),
         m_begin_y(begin_y),
         m_parent_cols(parent_cols),
         m_parent_lines(parent_lines),
         m_hor_ch(0),
         m_ver_ch(0),
         m_boxed(false),
         m_cursor_x(0),
         m_cursor_y(0)
    {
        m_window = newwin(
                    static_cast<int>(m_per_lines*m_parent_lines),
                    static_cast<int>(m_per_cols*m_parent_cols),
                    static_cast<int>(m_begin_y*m_parent_lines),
                    static_cast<int>(m_begin_x*m_parent_cols)
                    );
    }

    WINDOW* TWindow::operator*()
    {
        return m_window;
    }

    int TWindow::refresh()
    {
        return wrefresh(m_window);;
    }

    int TWindow::box(chtype ver_ch, chtype hor_ch)
    {
        m_ver_ch = ver_ch;
        m_hor_ch = hor_ch;
        m_boxed = true;
        return ::box(m_window, m_ver_ch, m_hor_ch);
    }

    int TWindow::mvwprintw(int y, int x, const std::string &text)
    {
        m_cursor_y = y;
        m_cursor_x = x;
        return ::mvwprintw(m_window, y, x, text.c_str());
    }

    int TWindow::mvwprintw(int y, int x, const std::string &&text)
    {
        m_cursor_y = y;
        m_cursor_x = x;
        return ::mvwprintw(m_window, y, x, text.c_str());
    }

    int TWindow::move(float y, float x)
    {
        m_begin_y = y;
        m_begin_x = x;
        return ::mvwin(m_window,
                       static_cast<int>(y*m_parent_lines),
                       static_cast<int>(x*m_parent_cols)
                       );
    }

    int TWindow::move()
    {
        return this->move(m_begin_y, m_begin_x);
    }

    int TWindow::resize(int parentlines, int parentcols)
    {
        m_parent_cols = parentcols;
        m_parent_lines = parentlines;
        return wresize(m_window,
                       static_cast<int>(m_per_lines*m_parent_lines),
                       static_cast<int>(m_per_cols*m_parent_cols)
                       );
    }

    int TWindow::erase()
    {
        return ::werase(m_window);
    }

    int TWindow::rebox()
    {
        if (!m_boxed)
            return OK;
        return this->box(m_ver_ch, m_hor_ch);
    }
}
