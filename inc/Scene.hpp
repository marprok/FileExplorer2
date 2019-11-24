#ifndef SCENE_H
#define SCENE_H

#include <ncurses.h>
#include <vector>
#include "ResizableWindow.hpp"

namespace view
{
    class Scene
    {
    private:
        std::vector<ResizableWindow> m_windows;
        size_t               m_input_window;
    public:
        Scene();
        ResizableWindow& operator[](size_t i);
        ~Scene();
        int refresh();
        void add_window(float perlines, float percols, float begin_y,
                        float begin_x);
        int resize();
        int erase();
        int rebox();
        int set_input_window(size_t i);
        ResizableWindow& get_input_window();
        int operator>>(int &key);
        std::string take_input(int lines, int cols, float begin_y,
                                     float begin_x, const std::string& prompt);
        std::string take_input(int lines, int cols, float begin_y,
                                     float begin_x, std::string&& prompt);
        bool ask(int lines, int cols, float begin_y,
                 float begin_x,  std::string& prompt);
        bool ask(int lines, int cols, float begin_y,
                 float begin_x,  std::string&& prompt);

        std::size_t choose(const std::vector<std::string> &choices,
                           std::size_t scroll_size,
                           std::string& prompt);
        std::size_t choose(const std::vector<std::string> &choices,
                           std::size_t scroll_size,
                           std::string&& prompt);
    };
}
#endif
