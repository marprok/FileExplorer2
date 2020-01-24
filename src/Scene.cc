#include "../inc/Scene.h"
#include <iostream>
#include <sstream>

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

    TerminalWindow& Scene::operator[](size_t i)
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
        m_windows.push_back(TerminalWindow{
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

    TerminalWindow& Scene::get_input_window()
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
}
