#ifndef DWINDOW_H
#define DWINDOW_H

#include <ncurses.h>
#include <string>
#include <vector>
#include "Scene.hpp"
#include "TerminalWindow.hpp"

namespace view
{
    class DialogWindow : public TerminalWindow
    {
    public:
        std::string take_input(const std::string &prompt);
        bool ask(const std::string &question);
        std::size_t choose(const std::vector<std::string> &choices);
        DialogWindow(const DialogWindow& other) = delete;
        DialogWindow& operator=(const DialogWindow& other) = delete;
        DialogWindow(DialogWindow&& other) = delete;
        DialogWindow& operator=(DialogWindow&& other) = delete;
        int resize() override;
        friend class Scene;
    private:
        DialogWindow(int lines, int cols, float y, float x);
    };
}
#endif
