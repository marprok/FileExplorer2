#ifndef TWINDOW_H
#define SCENE_H

#include <ncurses.h>
#include <vector>
#include "TWindow.hpp"
namespace view
{
    class Scene
    {
    private:
        int                  m_num_lines, m_num_cols;
        std::vector<TWindow> m_windows;
    public:
        Scene(int num_lines, int num_cols);
        TWindow& operator[](size_t i);
        int refresh();
        void add_window(float perlines, float percols, float begin_y, float begin_x, int parent_lines, int parent_cols);
        int resize(int num_lines, int num_cols);
        int erase();
    };
}
#endif
