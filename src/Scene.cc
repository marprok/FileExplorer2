#include "../inc/Scene.hpp"
#include <iostream>

namespace view
{
    Scene::Scene(int nlines, int ncols)
        :m_nlines(nlines), m_ncols(ncols)
    {

    }

    TWindow& Scene::operator[](Orientation i)
    {
        return m_windows.find(i)->second;
    }

    int Scene::refresh()
    {
        int ret = OK;
        for (auto& pair : m_windows)
        {
            if ((ret = pair.second.refresh()) != OK)
                return ret;
        }
        return ret;
    }

    void Scene::add_window(Orientation ori, int nlines, int ncols, int begin_y, int begin_x)
    {
        m_windows.emplace(ori, TWindow{nlines, ncols, begin_y, begin_x});
    }

    int Scene::resize(int nlines, int ncols)
    {
        int ret = OK;
        for (auto& pair : m_windows)
        {
            switch (pair.first)
            {
            case Orientation::LEFT:
                ret = pair.second.resize(nlines, ncols/2);
                if (ret != OK)
                    return ret;
                ret = pair.second.move(0, 0);
                if (ret != OK)
                    return ret;
                ret = pair.second.erase();
                if (ret != OK)
                    return ret;
                ret = pair.second.rebox();
                if (ret != OK)
                    return ret;
                break;
            case Orientation::RIGHT:
                ret = pair.second.resize(nlines, ncols/2);
                if (ret != OK)
                    return ret;
                ret = pair.second.move(0, ncols/2);
                if (ret != OK)
                    return ret;
                ret = pair.second.erase();
                if (ret != OK)
                    return ret;
                ret = pair.second.rebox();
                if (ret != OK)
                    return ret;
                break;
            }
        }
        return ret;
    }


    int Scene::erase()
    {
        int ret = OK;
        for (auto& pair : m_windows)
        {
            if ((ret = pair.second.erase()) != OK)
                return ret;
        }
        return ret;
    }
}
