#include <iostream>
#include <sstream>
#include <cmath>
#include "../inc/terminal_window.h"

namespace view
{
    TerminalWindow::TerminalWindow(float lines, float cols, float begin_y,
                                     float begin_x)
        :m_window(nullptr),
         m_per_cols(cols),
         m_per_lines(lines),
         m_begin_x(begin_x),
         m_begin_y(begin_y),
         m_hor_ch(0),
         m_ver_ch(0),
         m_boxed(false),
         m_cursor_x(0),
         m_cursor_y(0)
    {
        m_window = newwin(this->lines(),
                          this->cols(),
                          this->y(),
                          this->x());
    }

    TerminalWindow::TerminalWindow()
        :m_window(nullptr),
         m_per_cols(0.0f),
         m_per_lines(0.0f),
         m_begin_x(0.0f),
         m_begin_y(0.0f),
         m_hor_ch(0),
         m_ver_ch(0),
         m_boxed(false),
         m_cursor_x(0),
         m_cursor_y(0)
    {

    }

    int TerminalWindow::resize()
    {
        int ret;
        ret = this->erase();
        if (ret != OK)
            return ret;
        ret = this->virtual_refresh();
        if (ret != OK)
            return ret;
        ret = this->_resize();
        if (ret != OK)
            return ret;
        ret = this->move();
        if (ret != OK)
            return ret;
        ret = this->rebox();
        if (ret != OK)
            return ret;

        return OK;
    }

    int TerminalWindow::_resize()
    {
        return wresize(operator*(),
                       this->lines(),
                       this->cols()
                       );
    }

    WINDOW* TerminalWindow::operator*()
    {
        return m_window;
    }

    int TerminalWindow::refresh()
    {
        return wrefresh(m_window);;
    }

    int TerminalWindow::virtual_refresh()
    {
        return wnoutrefresh(m_window);
    }

    int TerminalWindow::box(chtype ver_ch, chtype hor_ch)
    {
        m_ver_ch = ver_ch;
        m_hor_ch = hor_ch;
        m_boxed = true;
        return ::box(m_window, m_ver_ch, m_hor_ch);
    }

    int TerminalWindow::mvwprintw(int y, int x, const std::string &text)
    {
        m_cursor_y = y;
        m_cursor_x = x;
        return ::mvwprintw(m_window, y, x, text.c_str());
    }

    int TerminalWindow::print(const std::string& text, attr_t attr)
    {
        int text_len = static_cast<int>(text.size());
        int win_cols = cols();
        if (win_cols < text_len)
            return ERR;

        wattron(m_window, attr);
        int ret = ::wprintw(m_window, text.c_str());
        wattroff(m_window, attr);

        return ret;
    }

    int TerminalWindow::print_left(int line, const std::string& text, attr_t attr)
    {
        int  col = 0;
        int text_len = static_cast<int>(text.size());
        int win_cols = cols();
        int win_lines = lines();

        if (boxed())
        {
            col++;
            win_lines--;
            win_cols--;
        }

        if (win_lines < line || win_cols < text_len)
            return ERR;

        wattron(m_window, attr);
        int ret = mvwprintw(line, col, text.c_str());
        wattroff(m_window, attr);

        return ret;
    }

    int TerminalWindow::print_right(int line, const std::string& text, attr_t attr)
    {
        int  col = 0;
        int text_len = static_cast<int>(text.size());
        int win_cols = cols();
        int win_lines = lines();

        if (boxed())
        {
            col++;
            win_lines--;
            win_cols--;
        }

        if (win_lines < line || win_cols < text_len)
            return ERR;

        wattron(m_window, attr);
        int ret = mvwprintw(line, win_cols - text_len, text.c_str());
        wattroff(m_window, attr);

        return ret;
    }

    int TerminalWindow::print_center(int line, const std::string& text, attr_t attr)
    {
        int  col = 0;
        int text_len = static_cast<int>(text.size());
        int win_cols = cols();
        int win_lines = lines();

        if (boxed())
        {
            col++;
            win_lines--;
            win_cols--;
        }

        if (win_lines < line || win_cols < text_len)
            return ERR;

        wattron(m_window, attr);
        int ret = mvwprintw(line, (win_cols - text_len)/2, text.c_str());
        wattroff(m_window, attr);

        return ret;
    }

    int TerminalWindow::move(float y, float x)
    {
        m_begin_y = y;
        m_begin_x = x;
        if (LINES <= 1)
        {
            return ::mvwin(m_window, 1, COLS);
        }else if (COLS <= 1)
        {
            return ::mvwin(m_window, LINES, 1);
        }else
        {
            return ::mvwin(m_window,
                           this->y(),
                           this->x());
        }

    }

    int TerminalWindow::move()
    {
        return TerminalWindow::move(m_begin_y,
                                     m_begin_x);
    }

    int TerminalWindow::erase()
    {
        return ::werase(m_window);
    }

    int TerminalWindow::rebox()
    {
        if (!m_boxed)
            return OK;
        return this->box(m_ver_ch, m_hor_ch);
    }

    int TerminalWindow::move_cursor(int y, int x)
    {
        return wmove(m_window, y, x);
    }

    int TerminalWindow::cols() const
    {
        return static_cast<int>(std::floor(m_per_cols*COLS));
    }

    int TerminalWindow::lines() const
    {
        return static_cast<int>(std::floor(m_per_lines*LINES));
    }

    int TerminalWindow::y() const
    {
        return static_cast<int>(std::ceil(m_begin_y*LINES));
    }

    int TerminalWindow::x() const
    {
        return static_cast<int>(std::ceil(m_begin_x*COLS));
    }

    void TerminalWindow::set_cols(float cols)
    {
        this->m_per_cols = cols;
    }

    void TerminalWindow::set_lines(float lines)
    {
        this->m_per_lines = lines;
    }

    bool TerminalWindow::boxed() const
    {
        return m_boxed;
    }

    int TerminalWindow::delwin()
    {
        return ::delwin(m_window);
    }

    void TerminalWindow::reset(float lines, float cols, float begin_y,
                                float begin_x)
    {
        m_per_lines = lines;
        m_per_cols= cols;
        m_begin_x = begin_x;
        m_begin_y = begin_y;
        m_hor_ch = 0;
        m_ver_ch = 0;
        m_boxed = false;
        m_cursor_x = 0;
        m_cursor_y = 0;

        if (m_window)
            this->delwin();

        m_window = newwin(this->lines(),
                          this->cols(),
                          this->y(),
                          this->x());
    }
}
