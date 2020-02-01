#include "../inc/scene.h"
#include <ncurses.h>

namespace view
{
    Scene::Scene()
         :m_input_window(0)
    {
        init_ncureses();
    }

    Scene::~Scene()
    {
        for (auto& window : m_windows)
            window.delwin();
        endwin();
    }

    void Scene::init_ncureses()
    {
        /* Global curses initialization */
        initscr();
        cbreak();
        noecho();
        curs_set(0); /* Make the cursor invisible. */
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
    }

    Terminal_window& Scene::operator[](size_t i)
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
        m_windows.push_back(Terminal_window{
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

    Terminal_window& Scene::get_input_window()
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

    std::string Scene::take_input(float lines, float cols, float begin_y,
                                  float begin_x, const std::string& prompt)
    {
        this->erase();
        this->refresh();
        curs_set(1);
        Terminal_window win(lines, cols, begin_y, begin_x);
        std::string out = m_query_manager.take_input(win, prompt);
        win.delwin();
        /* reset the state of the current scene */
        curs_set(0);
        keypad(*get_input_window(), true);
        this->resize();
        this->refresh();

        return out;
    }

    std::string Scene::take_input(float lines, float cols, float begin_y,
                                  float begin_x, const std::string&& prompt)
    {
        return take_input(lines, cols, begin_y, begin_x, prompt);
    }

    bool Scene::ask(float lines, float cols, float begin_y,
             float begin_x, const std::string &question)
    {
        /* clear the state of the current scene */
        this->erase();
        this->refresh();
        Terminal_window win(lines, cols,
                         begin_y, begin_x);
        bool choice = m_query_manager.ask(win, question);
        win.delwin();
        keypad(*get_input_window(), true);
        this->resize();
        this->refresh();

        return choice;
    }

    bool Scene::ask(float lines, float cols, float begin_y,
             float begin_x, const std::string &&question)
    {
        return ask(lines, cols, begin_y, begin_x, question);
    }
}
