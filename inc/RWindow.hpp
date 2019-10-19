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
        int _resize();
        RWindow(float per_lines, float per_cols, float begin_y,
                float begin_x);
    public:
        RWindow(const RWindow& other);
        RWindow& operator=(const RWindow& other);
        RWindow(RWindow&& other);
        RWindow& operator=(RWindow&& other);

        int resize() override;
        friend class Scene;
    };
}
#endif
