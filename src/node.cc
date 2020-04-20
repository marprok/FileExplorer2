#include <algorithm>
#include <cstdio> // rename
#include <sys/stat.h> // mkdir
#include <dirent.h> // opendir
#include <fcntl.h> // creat
#include "../inc/node.h"

namespace fs
{

    Node::Node(const Inode& inode, Node* parent)
        :m_parent(parent), m_inode(std::move(inode)),m_loaded(false)
    {
        _update_abs_path();
        m_inode.stat(abs_path());
    }

    Node::~Node()
    {
        for (auto& dir : m_dirs)
            delete dir;

        for (auto& file : m_files)
            delete file;
    }

    bool Node::is_ancestor_of(const Node* other) const
    {
        assert(other);
        while (other)
        {
            if (*other == *this)
                return true;
            other = other->m_parent;
        }

        return false;
    }

    std::size_t Node::load()
    {
        if (!m_inode.is_directory() || m_loaded)
            return size();
        m_loaded = true;

        DIR *dir = dir = opendir(abs_path().c_str());
        struct dirent *drt;

        if (!dir)
        {
            std::cerr << "ERROR: opendir" << std::endl << "Directory abs name: "<< abs_path() << std::endl;
            return 0;
        }

        while ((drt = readdir(dir)))
        {
            if (drt->d_type != DT_DIR)
            {
                /* This is not a directory. */
                m_files.push_back(new Node({drt->d_name}, this));
            }else
            {
                /* In case the m_name is . or .. */
                size_t temp = strlen(drt->d_name);
                if ((temp == 1 && drt->d_name[0] == '.') ||
                    (temp == 2 && drt->d_name[0] == '.' && drt->d_name[1] == '.'))
                {
                    continue;
                }
                m_dirs.push_back(new Node({drt->d_name}, this));
            }
        }

        closedir(dir);
        return size();
    }

    Node* Node::parent() { return m_parent; }

    Inode& Node::inode() { return m_inode; }

    std::vector<Node*>& Node::dirs() { return m_dirs; }

    std::vector<Node*>& Node::files() { return m_files; }

    std::size_t Node::size() const { return m_dirs.size() + m_files.size(); }

    bool Node::empty() { return size() == 0; }

    void Node::_update_abs_path()
    {
        m_abs_path = m_parent == nullptr ? m_inode.name()  : m_parent->abs_path() + "/" + m_inode.name();

        if (m_loaded)
        {
            for (auto& dir : m_dirs)
                dir->_update_abs_path();

            for (auto& file : m_files)
                file->_update_abs_path();
        }
    }

    const std::string& Node::abs_path() const
    {
        return m_abs_path;
    }

    bool Node::operator==(const Node& other) const
    {
        return abs_path() == other.abs_path();
    }

    void Node::move(Node *new_parent)
    {
        if (!new_parent || *this == *new_parent || is_ancestor_of(new_parent))
            return;

        std::string new_abs_path = new_parent->abs_path() + "/" + m_inode.name();
        if (rename(abs_path().c_str(), new_abs_path.c_str()) < 0 )
            return; // maybe log it

        if (m_inode.is_directory())
        {
            auto this_iter = std::find_if(m_parent->m_dirs.begin(), m_parent->m_dirs.end(), [this] (const auto& other) { return  this->m_abs_path == other->m_abs_path; });
            if (this_iter != m_parent->m_dirs.end())
                m_parent->m_dirs.erase(this_iter);
            new_parent->m_dirs.push_back(this);
        }else
        {
            auto this_iter = std::find_if(m_parent->m_files.begin(), m_parent->m_files.end(), [this] (const auto& other) { return  this->m_abs_path == other->m_abs_path; });
            if (this_iter != m_parent->m_files.end())
                m_parent->m_files.erase(this_iter);
            new_parent->m_files.push_back(this);
        }

        m_parent = new_parent;
        _update_abs_path();
        m_inode.stat(abs_path());
    }

    bool Node::_remove()
    {
        load();
        for (auto& dir : m_dirs)
        {
            if (!dir->_remove())
                return false;
        }

        for (auto& file : m_files)
        {
            if (!file->_remove())
                return false;
        }

        if (m_inode.is_directory())
        {
            if (rmdir(abs_path().c_str()) < 0)
                return false;
        }
        else
        {
            if (unlink(abs_path().c_str()) < 0)
                return false;
        }

        m_parent = nullptr;
        return true;
    }

    bool Node::remove()
    {
        if (m_parent)
        {
            if (m_inode.is_directory())
            {
                auto this_iter = std::find_if(m_parent->m_dirs.begin(), m_parent->m_dirs.end(), [this] (const auto& other) { return  this->m_abs_path == other->m_abs_path; });
                if (this_iter != m_parent->m_dirs.end())
                    m_parent->m_dirs.erase(this_iter);
            }else
            {
                auto this_iter = std::find_if(m_parent->m_files.begin(), m_parent->m_files.end(), [this] (const auto& other) { return  this->m_abs_path == other->m_abs_path; });
                if (this_iter != m_parent->m_files.end())
                    m_parent->m_files.erase(this_iter);
            }
        }
        return _remove();
    }

    bool Node::create_dir(const std::string &name)
    {
        std::string path = m_abs_path + "/" + name;

        if (mkdir(path.c_str(), 0755) < 0)
            return false;

        m_dirs.push_back(new Node({name}, this));

        return true;
    }

    bool Node::create_file(const std::string &name)
    {
        std::string path = m_abs_path + "/" + name;

        if (creat(path.c_str(), 0644) < 0)
            return false;

        m_files.push_back(new Node({name}, this));

        return true;
    }
}
