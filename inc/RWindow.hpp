#ifndef RWINDOW_H
#define RWINDOW_H

#include <ncurses.h>
#include <string>
#include "TWindow.hpp"

namespace view
{
    class RWindow : public TWindow
    {
    private:
        float   m_per_cols, m_per_lines;
        float   m_begin_x, m_begin_y;
        int     m_scene_lines, m_scene_cols;
        int     m_parent_lines, m_parent_cols;
        int _resize(int parentlines, int parentcols);
    public:
        RWindow(float per_lines, float per_cols, float begin_y,
                float begin_x, int scene_lines, int scene_cols);
        int resize(int scene_lines, int scene_cols);
        int move();
    };
}
#endif
