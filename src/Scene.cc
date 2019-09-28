#include "../inc/Scene.hpp"
#include <iostream>

namespace view
{
    Scene::Scene(int num_lines, int num_cols)
        :m_num_lines(num_lines), m_num_cols(num_cols)
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
        int ret = OK;
        m_num_cols = num_cols;
        m_num_lines = num_lines;
        for (auto& window : m_windows)
        {
                ret = window.resize(num_lines, num_cols);
                if (ret != OK)
                    return ret;
                ret = window.move();
                if (ret != OK)
                    return ret;
                ret = window.erase();
                if (ret != OK)
                    return ret;
                ret = window.rebox();
                if (ret != OK)
                    return ret;
        }
        return ret;
    }


    int Scene::erase()
    {
        int ret = OK;
        for (auto& window : m_windows)
        {
            if ((ret = window.erase()) != OK)
                return ret;
        }
        return ret;
    }
}
