#ifndef DWINDOW_H
#define DWINDOW_H

#include <ncurses.h>
#include <string>
#include <vector>

namespace view
{
    class DWindow
    {
    public:
        DWindow(int lines, int cols, int y, int x);
        std::string take_input(const std::string &prompt);
        bool ask(const std::string &question);
        std::size_t choose(const std::vector<std::string> &choices);
    };
}
#endif
