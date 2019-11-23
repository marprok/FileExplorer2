#ifndef DWINDOW_H
#define DWINDOW_H

#include <ncurses.h>
#include <string>
#include <vector>
#include "Scene.hpp"
#include "TWindow.hpp"

namespace view
{
    class DWindow : public TWindow
    {
    public:
        std::string take_input(const std::string &prompt);
        bool ask(const std::string &question);
        std::size_t choose(const std::vector<std::string> &choices);
        DWindow(const DWindow& other) = delete;
        DWindow& operator=(const DWindow& other) = delete;
        DWindow(DWindow&& other) = delete;
        DWindow& operator=(DWindow&& other) = delete;
        int resize() override;
        friend class Scene;
    private:
        DWindow(int lines, int cols, float y, float x);
    };
}
#endif
