#ifndef TWINDOW_H
#define SCENE_H

#include <ncurses.h>
#include <map>
#include "TWindow.hpp"
namespace view
{
    enum class Orientation
    {
        NONE,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };
    class Scene
    {
    private:
        int m_nlines, m_ncols;
        std::map<Orientation, TWindow> m_windows;
    public:
        Scene(int nlines, int ncols);
        TWindow& operator[](Orientation i);
        int refresh();
        void add_window(Orientation ori, int nlines, int ncols, int begin_y, int begin_x);
        int resize(int nlines, int ncols);
        int erase();
    };
}
#endif
