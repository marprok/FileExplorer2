#ifndef FS_NODE_H
#define FS_NODE_H
#include "inode.h"
#include "linked_list.hpp"

namespace fs {

class Node
{
private:
    Node* m_parent;
    Inode* m_inode;
    Linked_list<Node> m_dirs;
    Linked_list<Node> m_files;
    bool m_loaded;
public:
    Node(Inode* data, Node* parent);
    ~Node();
    Node* parent() const { return m_parent; }
    Inode* inode() const { return m_inode; }
    Linked_list<Node>& dirs() { return m_dirs; }
    Linked_list<Node>& files() { return m_files; }
    std::size_t size() const { return m_dirs.size() + m_files.size(); }
    bool empty() { return size() == 0; }

    std::string abs_path() const
    {
        return m_parent == nullptr ? m_inode->name()  : m_parent->abs_path() + "/" + m_inode->name();
    }

    bool operator==(const Node& other) const
    {
        return abs_path() == other.abs_path();
    }

    bool is_ancestor_of(const Node* other) const;

    std::size_t load();
    void copy(Node* new_parent);
    void move(Node* new_parent);
    void remove();
};
}

#endif
