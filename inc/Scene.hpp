#ifndef TWINDOW_H
#define SCENE_H

#include <ncurses.h>
#include <vector>
#include "RWindow.hpp"

namespace view
{
    class Scene
    {
    private:
        int                  m_num_lines, m_num_cols;
        std::vector<RWindow> m_windows;
        size_t               m_input_window;
    public:
        Scene(int num_lines, int num_cols);
        RWindow& operator[](size_t i);
        ~Scene();
        int refresh();
        void add_window(float perlines, float percols, float begin_y,
                        float begin_x, int parent_lines, int parent_cols);
        int resize(int num_lines, int num_cols);
        int erase();
        int rebox();
        int set_input_window(size_t i);
        RWindow& get_input_window();
        int operator>>(int &key);
        std::string crt_input_window(float perlines, float percols, float begin_y,
                                     float begin_x, const std::string& msg);
        std::string crt_input_window(float perlines, float percols, float begin_y,
                                     float begin_x, const std::string&& msg);
        int crt_yn_window(float begin_y, float begin_x,
                          const std::string& msg);
    };
}
#endif
