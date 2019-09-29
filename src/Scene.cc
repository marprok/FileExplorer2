#include "../inc/Scene.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    Scene::Scene(int num_lines, int num_cols)
        :m_num_lines(num_lines),
         m_num_cols(num_cols),
         m_input_window(0)
    {

    }

    TWindow& Scene::operator[](size_t i)
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
                           float begin_x, int parent_lines, int parent_cols)
    {
        m_windows.emplace_back(perlines,
                               percols,
                               begin_y,
                               begin_x,
                               parent_lines,
                               parent_cols
                               );
    }

    int Scene::resize(int num_lines, int num_cols)
    {
        int ret;
        m_num_cols = num_cols;
        m_num_lines = num_lines;
        for (auto& window : m_windows)
        {
            ret = window.resize(m_num_lines, m_num_cols);
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

    int Scene::set_input_window(size_t i)
    {
        if (i >= m_windows.size())
            return ERR;
        m_input_window = i;
        return OK;
    }

    TWindow& Scene::get_input_window()
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

    std::string Scene::crt_input_window(float perlines, float percols, float begin_y,
                                        float begin_x, std::string& msg)
    {
        /* clear the state of the current scene */
        this->erase();
        this->refresh();
        TWindow temp(perlines, percols, begin_y,
                     begin_x, m_num_lines, m_num_cols);
        temp.box('#','#');
        curs_set(1); /* Make the cursor visible. */
        keypad(*temp, true);

        int x_pos = static_cast<int>((m_num_cols*percols)/2 - msg.size()/2);
        x_pos = x_pos > 0 ? x_pos : 1;

        temp.mvwprintw(1,x_pos, msg);
        temp.move_cursor(2,1);
        temp.refresh();
        /* Handle the keyboard input */
        std::string text;
        int key;
        while ((key = wgetch(*temp)) != '\n')
        {
            switch (key)
            {
            case KEY_BACKSPACE:
                if (text.size() > 0 )
                    text = text.substr(0, text.size() - 1);
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, m_num_lines, m_num_cols);
                temp.resize(m_num_lines, m_num_cols);
                x_pos = static_cast<int>((m_num_cols*percols)/2 - msg.size()/2);
                x_pos = x_pos > 0 ? x_pos : 1;
                break;
            default:
                if (text.size() + 2 < percols*m_num_cols)
                    text += static_cast<char>(key);
                break;
            }
            temp.erase();
            temp.refresh();
            temp.rebox();
            temp.mvwprintw(1,x_pos, msg);
            temp.mvwprintw(2,1, text);
            temp.refresh();
        }
        /* Bring back the state of the scene */
        temp.erase();
        temp.refresh();
        delwin(*temp);
        curs_set(0);
        this->resize(m_num_lines, m_num_cols);
        this->refresh();

        return text;
    }

    std::string Scene::crt_input_window(float perlines, float percols, float begin_y,
                                 float begin_x, std::string&& msg)
    {
        return this->crt_input_window(perlines, percols, begin_y,
                                      begin_x, msg);
    }
}
