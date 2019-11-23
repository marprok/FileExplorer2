#ifndef RWINDOW_H
#define RWINDOW_H

#include <ncurses.h>
#include <string>
#include "TerminalWindow.hpp"

namespace view
{
    class ResizableWindow : public TerminalWindow
    {
    private:
        float   m_per_cols, m_per_lines;
        int _resize();
        ResizableWindow(float per_lines, float per_cols, float begin_y,
                float begin_x);
    public:
        ResizableWindow(const ResizableWindow& other);
        ResizableWindow& operator=(const ResizableWindow& other);
        ResizableWindow(ResizableWindow&& other);
        ResizableWindow& operator=(ResizableWindow&& other);

        int resize() override;
        friend class Scene;
    };
}
#endif
