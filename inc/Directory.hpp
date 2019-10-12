#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <vector>
#include <string>
#include <memory>
#include "File.hpp"

namespace fs
{
    class Directory
    {
    public:
    private:
        std::string                m_name;
        Directory*                 m_parent;
        std::size_t                m_size;
        bool                       m_loaded;
        std::size_t                m_index;
        std::vector<File>          m_files;
        std::vector<Directory*>    m_dirs;
    public:
        Directory(const std::string& name, Directory* parent);
        std::size_t load_info();
        std::size_t reload_info();
        ~Directory();
        Directory* dive(std::size_t i);
        Directory* surface();
        std::string abs_path() const;
        const std::string name() const;
        bool loaded() const;
        std::size_t index() const;
        std::vector<File>& files();
        std::vector<Directory*>& dirs();
        bool empty() const;
        int create_file(const std::string& name);
    };
}
#endif // DIRECTORY_H
