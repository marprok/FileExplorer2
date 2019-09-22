#include "../inc/TWindow.hpp"
#include <iostream>

namespace view
{
    TWindow::TWindow(int nlines, int ncols, int begin_y, int begin_x)
        :m_window(nullptr),
         m_ncols(ncols),
         m_nlines(nlines),
         m_begin_x(begin_x),
         m_begin_y(begin_y),
         m_horch(0),
         m_verch(0),
         m_boxed(false),
         m_cursorx(0),
         m_cursory(0)
    {
        std::cout << "Created" << std::endl;
        m_window = newwin(m_nlines, m_ncols, m_begin_y, m_begin_x);
    }

    WINDOW* TWindow::operator*()
    {
        return m_window;
    }

    int TWindow::refresh()
    {
        return wrefresh(m_window);;
    }

    int TWindow::box(chtype verch, chtype horch)
    {
        m_verch = verch;
        m_horch = horch;
        m_boxed = true;
        return ::box(m_window, verch, horch);
    }

    int TWindow::mvwprintw(int y, int x, const std::string &text)
    {
        m_cursory = y;
        m_cursorx = x;
        return ::mvwprintw(m_window, y, x, text.c_str());
    }

    int TWindow::mvwprintw(int y, int x, const std::string &&text)
    {
        m_cursory = y;
        m_cursorx = x;
        return ::mvwprintw(m_window, y, x, text.c_str());
    }

    int TWindow::move(int y, int x)
    {
        m_begin_y = y;
        m_begin_x = x;
        //wmove(m_window, 1,1);
        return ::mvwin(m_window, y, x);
    }

    int TWindow::resize(int nlines, int ncols)
    {
        return wresize(m_window, nlines, ncols);
    }

    int TWindow::erase()
    {
        return ::werase(m_window);
    }

    int TWindow::rebox()
    {
        if (!m_boxed)
            return OK;
        return this->box(m_verch, m_horch);
    }
}
