#include <cstdio> // rename
#include "../inc/node.h"

namespace fs
{

Node::Node(Inode* inode, Node* parent)
    :m_parent(parent), m_inode(inode),m_loaded(false)
{
    m_inode->stat(abs_path());
}

Node::~Node()
{
    if (m_inode)
        delete m_inode;
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
    if (m_loaded)
        return size();
    m_loaded = true;
    return m_inode->populate(this);
}

void Node::copy(Node *new_parent)
{
    (void)new_parent;
}

void Node::move(Node *new_parent)
{
    if (!new_parent || *this == *new_parent || is_ancestor_of(new_parent))
        return;

    std::string new_abs_path = new_parent->abs_path() + "/" + m_inode->name();
    if (rename(abs_path().c_str(), new_abs_path.c_str()) < 0 )
        return; // maybe log it

    if (m_inode->is_directory())
        m_parent->m_dirs.move(new_parent->dirs(), this);
    else
        m_parent->m_files.move(new_parent->files(), this);

    m_parent = new_parent;
    m_inode->stat(abs_path());
}

void Node::remove(Node* child)
{
    Linked_list<Node>::Link *link = nullptr;
    if (child->m_inode->is_directory())
        link = m_dirs.unlink(child);
    else
        link = m_files.unlink(child);

    if (!link)
        return;

    child->load();

    Linked_list<Node>::Link *head = child->m_dirs.head();
    while (head)
    {
        auto tmp = head->next();
        child->remove(head->data());
        head = tmp;
    }

    head = child->m_files.head();
    while (head)
    {
        auto tmp = head->next();
        child->remove(head->data());
        head = tmp;
    }
    // ugly, but the inode needs a way to get it's full path...
    child->m_inode->remove(child);

    child->m_parent = nullptr;
    delete link;
}

}
