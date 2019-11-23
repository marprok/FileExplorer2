#include "../inc/TWindow.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    TWindow::TWindow(int lines, int cols, float begin_y,
                     float begin_x)
        :m_window(nullptr),
         m_lines(lines),
         m_cols(cols),
         m_begin_x(begin_x),
         m_begin_y(begin_y),
         m_hor_ch(0),
         m_ver_ch(0),
         m_boxed(false),
         m_cursor_x(0),
         m_cursor_y(0)
    {
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        m_window = newwin(
                    m_lines,
                    m_cols,
                    static_cast<int>(m_begin_y*scene_lines),
                    static_cast<int>(m_begin_x*scene_cols)
                    );
    }

    TWindow::TWindow()
        :m_window(nullptr),
          m_lines(0),
          m_cols(0),
          m_begin_x(0.0f),
          m_begin_y(0.0f),
          m_hor_ch(0),
          m_ver_ch(0),
          m_boxed(false),
          m_cursor_x(0),
          m_cursor_y(0)
    {

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

    int TWindow::print(const std::string& text)
    {
        return ::wprintw(m_window, text.c_str());
    }

    int TWindow::print(const std::string&& text)
    {
        return ::wprintw(m_window, text.c_str());
    }

    int TWindow::move(float y, float x)
    {
        m_begin_y = y;
        m_begin_x = x;
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        if (scene_lines <= 1)
        {
            return ::mvwin(m_window, 1, scene_cols);
        }else if (scene_cols <= 1)
        {
            return ::mvwin(m_window, scene_lines, 1);
        }else
        {
            return ::mvwin(m_window,
                           static_cast<int>(m_begin_y*scene_lines),
                           static_cast<int>(m_begin_x*scene_cols)
                           );
        }

    }

    int TWindow::move()
    {
        return TWindow::move(
                    m_begin_y,
                    m_begin_x
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

    int TWindow::move_cursor(int y, int x)
    {
        return wmove(m_window, y, x);
    }

    int TWindow::cols() { return m_cols; }
    int TWindow::lines() { return m_lines; }

    int TWindow::delwin()
    {
        return ::delwin(m_window);
    }

    void TWindow::reset(int lines, int cols, float begin_y,
                        float begin_x)
    {
        m_lines = lines;
        m_cols= cols;
        m_begin_x = begin_x;
        m_begin_y = begin_y;
        m_hor_ch = 0;
        m_ver_ch = 0;
        m_boxed = false;
        m_cursor_x = 0;
        m_cursor_y = 0;
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        if (m_window)
        {
            this->delwin();
        }
        m_window = newwin(
                        m_lines,
                        m_cols,
                        static_cast<int>(m_begin_y*scene_lines),
                        static_cast<int>(m_begin_x*scene_cols)
                    );
    }
}
