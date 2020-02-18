#include <dirent.h>
#include <iostream>
#include <cstring>
#include <fcntl.h> /* creat */
#include <unistd.h> /* unlink */
#include <algorithm>
#include "../inc/Directory.h"

namespace fs
{
    Directory::Directory(const std::string& name, Directory *parent)
        :m_name(name), m_parent(parent), m_size(0), m_loaded(false), m_index(0)
    {
        //load_info();
    }

    std::size_t Directory::load_info()
    {
        if (m_loaded)
        {
            return m_size;
        }

        DIR *dir = dir = opendir(abs_path().c_str());

        struct dirent *drt;

        if (!dir)
        {
            std::cerr << "ERROR: opendir" << std::endl << "Directory name: "<< m_name << std::endl;
            return 0;
        }

        while ((drt = readdir(dir)))
        {
            ++m_size;
            if (drt->d_type != DT_DIR)
            {
                /* This is not a directory. */
                m_files.emplace_back(drt->d_name, abs_path());
            }else
            {
                /* In case the m_name is . or .. */
                size_t temp = strlen(drt->d_name);
                if ((temp == 1 && drt->d_name[0] == '.') ||
                        (temp == 2 && drt->d_name[0] == '.' && drt->d_name[1] == '.'))
                {
                    --m_size;
                    continue;
                }
                m_dirs.push_back(new Directory(drt->d_name, this));
            }
        }

        closedir(dir);
        m_loaded = true;
        return m_size;
    }

    std::size_t Directory::reload_info()
    {
        unload_info();
        return load_info();
    }

    void Directory::unload_info()
    {
        if (!loaded())
            return;

        if (!m_dirs.empty())
        {
            for (auto& d : m_dirs)
                delete d;
            m_dirs.clear();
        }

        if (!m_files.empty())
            m_files.clear();

        m_size = 0;
        m_loaded = false;
    }

    Directory::~Directory()
    {
        unload_info();
    }

    Directory* Directory::dive(std::size_t i)
    {
        m_index = i;
        for (std::size_t j = 0; j < m_dirs.size(); ++j)
            if (i != j && m_dirs[j]->loaded())
               m_dirs[j]->unload_info();
        return m_dirs[i];
    }

    Directory* Directory::surface()
    {
        return m_parent != nullptr ? m_parent : this;
    }

    std::string Directory::abs_path() const
    {
        return m_parent == nullptr ? m_name : m_parent->abs_path() + "/" + m_name;
    }

    const std::string Directory::name() const { return m_name; }
    bool Directory::loaded() const             { return m_loaded; }
    std::size_t Directory::index() const       { return m_index; }
    std::vector<File>& Directory::files()      { return m_files; }
    std::vector<Directory*>& Directory::dirs() { return m_dirs; }
    bool Directory::empty() const { return m_dirs.empty() && m_files.empty(); }

    int Directory::create_file(const std::string& name)
    {
        std::string file_name = name;
        int ret = 0;
        if (name == "")
            file_name = "New_File";
        auto file = std::find_if(m_files.begin(), m_files.end(),[file_name](const File & file){ return file.name() == file_name;});
        if (file == m_files.end())
        {
            ret = creat((abs_path() + "/" + file_name).c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            m_files.emplace_back(file_name, abs_path());
            return ret;
        }
        return ret;
    }

    int Directory::unlink_file(std::size_t i)
    {
        if (i > m_files.size())
            return 0;
        std::string abs_path= m_files[i].abs_path();
        int ret = 0;
        auto file = std::find_if(m_files.begin(), m_files.end(),[abs_path](const File & file){ return file.abs_path() == abs_path;});
        if (file != m_files.end())
        {
            ret = unlink(abs_path.c_str());
            m_files.erase(file);
        }
        return ret;
    }

}
