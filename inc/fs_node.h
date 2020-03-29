#ifndef FS_NODE_H
#define FS_NODE_H
#include "fs_data.h"
#include "linked_list.hpp"

namespace fs {

class FS_Node
{
private:
    FS_Node* m_parent;
    Inode* m_inode;
    Inode_list<FS_Node> m_dirs;
    Inode_list<FS_Node> m_files;
    bool m_loaded;
public:
    FS_Node(Inode* data, FS_Node* parent);
    ~FS_Node();
    FS_Node* parent() const { return m_parent; }
    Inode* inode() const { return m_inode; }
    Inode_list<FS_Node>& dirs() { return m_dirs; }
    Inode_list<FS_Node>& files() { return m_files; }
    std::size_t size() const { return m_dirs.size() + m_files.size(); }
    bool empty() { return size() == 0; }

    std::string abs_path()
    {
        return m_parent == nullptr ? m_inode->name()  : m_parent->abs_path() + "/" + m_inode->name();
    }

    bool operator==(const FS_Node& other)
    {
        return m_inode->abs_path() == other.m_inode->abs_path();
    }
    std::size_t load();
    void copy(FS_Node* new_parent);
    void move(FS_Node* new_parent);
};
}

#endif
