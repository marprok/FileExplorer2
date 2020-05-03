#ifndef FS_NODE_H
#define FS_NODE_H
#include "inode.h"
#include <vector>

namespace fs {

    class Node
    {
    private:
        Node* m_parent;
        Inode m_inode;
        std::vector<Node*> m_dirs;
        std::vector<Node*> m_files;
        std::string m_abs_path;
        bool m_loaded;
    public:
        Node(const Inode& data, Node* parent);

        Node(const Node&) = delete;

        Node& operator=(const Node&) = delete;

        Node(Node&&) = default;

        Node& operator=(Node&&) = default;

        ~Node();

        std::size_t size() const;

        bool empty();

        const std::string& abs_path() const;

        bool operator==(const Node& other) const;

        bool is_ancestor_of(const Node* other) const;

        const Inode& inode();

        std::vector<Node*>& files();

        std::vector<Node*>& dirs();

        Node* parent();

        std::size_t load();

        bool move(Node *new_parent);

        bool remove();

        bool create_dir(const std::string &name);

        bool create_file(const std::string &name);

    private:
        void _update_abs_path();

        bool _remove();
    };
}

#endif
