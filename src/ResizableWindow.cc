#include "../inc/RWindow.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    RWindow::RWindow(float per_lines, float per_cols, float begin_y,
                     float begin_x)

        :TWindow(),
         m_per_cols(per_cols),
         m_per_lines(per_lines)
    {
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        TWindow::reset(
              static_cast<int>(m_per_lines*scene_lines),
              static_cast<int>(m_per_cols*scene_cols),
              begin_y,
              begin_x
             );
    }

    int RWindow::resize()
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

    int RWindow::_resize()
    {

        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        m_lines = static_cast<int>(m_per_lines*scene_lines);
        m_cols = static_cast<int>(m_per_cols*scene_cols);
        return wresize(operator*(),
                       m_lines,
                       m_cols
                       );
    }



    RWindow::RWindow(const RWindow& other)
        :TWindow(other)
    {
        m_per_cols = other.m_per_cols;
        m_per_lines = other.m_per_lines;
        m_begin_x = other.m_begin_x;
        m_begin_y = other.m_begin_y;
        m_window = other.m_window;
    }
    RWindow& RWindow::operator=(const RWindow& other)
    {
        TWindow::operator=(other);
        m_per_cols = other.m_per_cols;
        m_per_lines = other.m_per_lines;
        m_begin_x = other.m_begin_x;
        m_begin_y = other.m_begin_y;
        m_window = other.m_window;

        return *this;
    }
    RWindow::RWindow(RWindow&& other)
        :TWindow(other)
    {
        m_per_cols = std::move(other.m_per_cols);
        m_per_lines = std::move(other.m_per_lines);
        m_begin_x = std::move(other.m_begin_x);
        m_begin_y = std::move(other.m_begin_y);
        m_window = std::move(other.m_window);
    }
    RWindow& RWindow::operator=(RWindow&& other)
    {
        TWindow::operator=(other);
        m_per_cols = std::move(other.m_per_cols);
        m_per_lines = std::move(other.m_per_lines);
        m_begin_x = std::move(other.m_begin_x);
        m_begin_y = std::move(other.m_begin_y);
        m_window = std::move(other.m_window);

        return *this;
    }
}
