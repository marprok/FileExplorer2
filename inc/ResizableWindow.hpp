#ifndef RWINDOW_H
#define RWINDOW_H

#include <ncurses.h>
#include <string>
#include "TerminalWindow.hpp"

namespace view
{
/**
 * @brief The ResizableWindow class
 *  This class represents a terminal window
 *  that will be resized in case of a resize event.
 */
class ResizableWindow : public TerminalWindow
{
private:
    float   m_per_cols, m_per_lines;
    /**
     * @brief _resize This is the actual resize method
     * @return returns the value of the internal function
     */
    int _resize();
    ResizableWindow(float per_lines, float per_cols, float begin_y,
                    float begin_x);
public:
    ResizableWindow(const ResizableWindow& other);
    ResizableWindow& operator=(const ResizableWindow& other);
    ResizableWindow(ResizableWindow&& other);
    ResizableWindow& operator=(ResizableWindow&& other);
    /**
     * @brief resize general resize event callback
     * @return OK in case of success
     */
    int resize() override;
    friend class Scene;
};
}
#endif
