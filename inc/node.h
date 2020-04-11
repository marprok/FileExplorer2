#ifndef FS_NODE_H
#define FS_NODE_H
#include "inode.h"
#include "ordered_list.hpp"

namespace fs {

class Node
{
private:
    Node* m_parent;
    Inode* m_inode;
    std::string m_abs_path;
    utils::Ordered_list<Node*> m_dirs;
    utils::Ordered_list<Node*> m_files;
    bool m_loaded;

    void _update_abs_path();
public:
    Node(Inode* data, Node* parent);

    Node(const Node&) = delete;

    Node& operator=(const Node&) = delete;

    Node(Node&&) = default;

    Node& operator=(Node&&) = default;

    ~Node();

    Node* parent() const;

    Inode* inode() const;

    utils::Ordered_list<Node*>& dirs();

    utils::Ordered_list<Node*>& files();

    std::size_t size() const;

    bool empty();

    const std::string& abs_path() const;

    bool operator==(const Node& other) const;

    bool operator<(const Node& other) const;

    bool operator>(const Node& other) const;

    bool operator>=(const Node& other) const;

    bool operator<=(const Node& other) const;

    bool operator!=(const Node& other) const;

    bool is_ancestor_of(const Node* other) const;

    std::size_t load();

    void move(Node* new_parent);

    void remove();
};
}

#endif
