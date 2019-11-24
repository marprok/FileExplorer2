#include "../inc/Scene.hpp"
#include <iostream>
#include <sstream>
#include "../inc/DialogWindow.hpp"

namespace view
{
    Scene::Scene()
         :m_input_window(0)
    {
    }
    Scene::~Scene()
    {
        for (auto& window : m_windows)
            window.delwin();
    }

    ResizableWindow& Scene::operator[](size_t i)
    {
        return m_windows[i];
    }

    int Scene::refresh()
    {
        int ret = OK;
        for (auto& window : m_windows)
        {
            if ((ret = window.refresh()) != OK)
                return ret;
        }
        return ret;
    }

    void Scene::add_window(float perlines, float percols, float begin_y,
                           float begin_x)
    {
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        m_windows.push_back(ResizableWindow{
                                perlines,
                                percols,
                                begin_y,
                                begin_x
                               });
    }

    int Scene::resize()
    {
        int ret;
        for (auto& window : m_windows)
        {
            ret = window.resize();
            if (ret != OK)
                return ret;
        }
        return OK;
    }

    int Scene::erase()
    {
        int ret;
        for (auto& window : m_windows)
        {
            if ((ret = window.erase()) != OK)
                return ret;
        }
        return OK;
    }

    int Scene::rebox()
    {
        int ret;
        for (auto& window : m_windows)
        {
            if ((ret = window.rebox()) != OK)
                return ret;
        }
        return OK;
    }

    int Scene::set_input_window(size_t i)
    {
        if (i >= m_windows.size())
            return ERR;
        m_input_window = i;
        return OK;
    }

    ResizableWindow& Scene::get_input_window()
    {
        return m_windows[m_input_window];
    }

    int Scene::operator>>(int &key)
    {
        key = wgetch(*get_input_window());
        if (key != ERR)
            return OK;
        return key;
    }

    std::string Scene::crt_input_window(int lines, int cols, float begin_y,
                                        float begin_x, const std::string& prompt)
    {
        /* clear the state of the current scene */
        this->erase();
        this->refresh();
        curs_set(1);
        int scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        DialogWindow win(lines, cols, begin_y, begin_x);
        std::string out = win.take_input(prompt);
        delwin(*win);
        /* reset the state of the current scene */
        curs_set(0);
        this->resize();
        this->refresh();

        return out;
    }

    std::string Scene::crt_input_window(int lines, int cols, float begin_y,
                                        float begin_x,  std::string&& prompt)
    {
        return this->crt_input_window(lines, cols, begin_y,
                                      begin_x, prompt);
    }

    bool Scene::ask(int lines, int cols, float begin_y,
                    float begin_x,  std::string& prompt)
    {
        /* clear the state of the current scene */
        this->erase();
        this->refresh();
        DialogWindow win(lines, cols,
                    begin_y, begin_x);
        bool choice = win.ask(prompt);
        this->resize();
        this->refresh();
        return choice;
    }

    bool Scene::ask(int lines, int cols, float begin_y,
                    float begin_x,  std::string&& prompt)
    {
        return this->ask(lines, cols, begin_y,
                         begin_x, prompt);
    }

    std::size_t Scene::choose(const std::vector<std::string> &choices,
                       std::size_t scroll_size,
                       std::string& prompt)
    {
        /* clear the state of the current scene */
        this->erase();
        this->refresh();
        DialogWindow win(scroll_size + 2 + 1, 30,
                    0.4f, 0.35f);
        std::size_t choice = win.choose(choices, scroll_size, prompt);
        this->resize();
        this->refresh();
        return choice;
    }

    std::size_t Scene::choose(const std::vector<std::string> &choices,
                       std::size_t scroll_size,
                       std::string&& prompt)
    {
        return this->choose(choices, scroll_size, prompt);
    }
}
