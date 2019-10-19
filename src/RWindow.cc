#include "../inc/RWindow.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    RWindow::RWindow(float per_lines, float per_cols, float begin_y,
                     float begin_x, int parent_lines, int parent_cols)

        :TWindow(),
         m_per_cols(per_cols),
         m_per_lines(per_lines),
         m_begin_x(begin_x),
         m_begin_y(begin_y),
         m_parent_lines(parent_lines),
         m_parent_cols(parent_cols)
    {
        TWindow::reset(
              static_cast<int>(m_per_lines*m_parent_lines),
              static_cast<int>(m_per_cols*m_parent_cols),
              static_cast<int>(m_begin_y*m_parent_lines),
              static_cast<int>(m_begin_x*m_parent_cols)
             );
    }

    int RWindow::resize(int parentlines, int parentcols)
    {
        int ret;
        ret = this->erase();
        if (ret != OK)
            return ret;
        ret = this->refresh();
        if (ret != OK)
            return ret;
        ret = this->_resize(parentlines, parentcols);
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

    int RWindow::_resize(int parentlines, int parentcols)
    {
        m_parent_cols = parentcols;
        m_parent_lines = parentlines;
        m_lines = static_cast<int>(m_per_lines*m_parent_lines);
        m_cols = static_cast<int>(m_per_cols*m_parent_cols);
        return wresize(operator*(),
                       m_lines,
                       m_cols
                       );
    }


    int RWindow::move()
    {
        return TWindow::move(
                    static_cast<int>(m_begin_y*m_parent_lines),
                    static_cast<int>(m_begin_x*m_parent_cols)
                    );
    }
}
