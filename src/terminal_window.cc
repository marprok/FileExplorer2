#include <iostream>
#include <sstream>
#include "../inc/terminal_window.h"

namespace view
{
Terminal_window::Terminal_window(float lines, float cols, float begin_y,
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
    int scene_lines, scene_cols;
    getmaxyx(stdscr, scene_lines, scene_cols);
    m_window = newwin(
                static_cast<int>(m_per_lines*scene_lines),
                static_cast<int>(m_per_cols*scene_cols),
                static_cast<int>(m_begin_y*scene_lines),
                static_cast<int>(m_begin_x*scene_cols)
                );
}

Terminal_window::Terminal_window()
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

int Terminal_window::resize()
{
    int ret;
    ret = this->erase();
    if (ret != OK)
        return ret;
    ret = this->refresh();
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

int Terminal_window::_resize()
{
    int scene_lines, scene_cols;
    getmaxyx(stdscr, scene_lines, scene_cols);
    return wresize(operator*(),
                   static_cast<int>(m_per_lines*scene_lines),
                   static_cast<int>(m_per_cols*scene_cols)
                   );
}

WINDOW* Terminal_window::operator*()
{
    return m_window;
}

int Terminal_window::refresh()
{
    return wrefresh(m_window);;
}

int Terminal_window::box(chtype ver_ch, chtype hor_ch)
{
    m_ver_ch = ver_ch;
    m_hor_ch = hor_ch;
    m_boxed = true;
    return ::box(m_window, m_ver_ch, m_hor_ch);
}

int Terminal_window::mvwprintw(int y, int x, const std::string &text)
{
    m_cursor_y = y;
    m_cursor_x = x;
    return ::mvwprintw(m_window, y, x, text.c_str());
}

int Terminal_window::print(const std::string& text)
{
    return ::wprintw(m_window, text.c_str());
}

int Terminal_window::print_left(int line, const std::string& text)
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

    if (win_lines < line ||
            win_cols < text_len)
        return ERR;

    return mvwprintw(line, col, text.c_str());
}

int Terminal_window::print_right(int line, const std::string& text)
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

    if (win_lines < line ||
            win_cols < text_len)
        return ERR;

    return mvwprintw(line, win_cols - text_len, text.c_str());
}

int Terminal_window::print_center(int line, const std::string& text)
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

    if (win_lines < line ||
            win_cols < text_len)
        return ERR;

    return mvwprintw(line, (win_cols - text_len)/2, text.c_str());
}

int Terminal_window::move(float y, float x)
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

int Terminal_window::move()
{
    return Terminal_window::move(
                m_begin_y,
                m_begin_x
                );
}

int Terminal_window::erase()
{
    return ::werase(m_window);
}

int Terminal_window::rebox()
{
    if (!m_boxed)
        return OK;
    return this->box(m_ver_ch, m_hor_ch);
}

int Terminal_window::move_cursor(int y, int x)
{
    return wmove(m_window, y, x);
}

int Terminal_window::cols() const
{
    int scene_lines, scene_cols;
    getmaxyx(stdscr, scene_lines, scene_cols);
    return static_cast<int>(m_per_cols*scene_cols);
}
int Terminal_window::lines() const
{
    int scene_lines, scene_cols;
    getmaxyx(stdscr, scene_lines, scene_cols);
    return static_cast<int>(m_per_lines*scene_lines);
}

void Terminal_window::set_cols(float cols)
{
    this->m_per_cols = cols;
}

void Terminal_window::set_lines(float lines)
{
    this->m_per_lines = lines;
}

bool Terminal_window::boxed() const
{
    return m_boxed;
}

int Terminal_window::delwin()
{
    return ::delwin(m_window);
}

void Terminal_window::reset(float lines, float cols, float begin_y,
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
    int scene_lines, scene_cols;
    getmaxyx(stdscr, scene_lines, scene_cols);
    if (m_window)
    {
        this->delwin();
    }
    m_window = newwin(
                static_cast<int>(m_per_lines*scene_lines),
                static_cast<int>(m_per_cols*scene_cols),
                static_cast<int>(m_begin_y*scene_lines),
                static_cast<int>(m_begin_x*scene_cols)
                );
}
}
