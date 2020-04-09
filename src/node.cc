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

    auto tmp = m_files.head();
    while (tmp)
    {
        if (tmp->data())
            delete tmp->data();
        tmp = tmp->next();
    }

    tmp = m_dirs.head();
    while (tmp)
    {
        if (tmp->data())
            delete tmp->data();
        tmp = tmp->next();
    }
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

Node* Node::parent() const { return m_parent; }
Inode* Node::inode() const { return m_inode; }
utils::Ordered_list<Node*>& Node::dirs() { return m_dirs; }
utils::Ordered_list<Node*>& Node::files() { return m_files; }
std::size_t Node::size() const { return m_dirs.size() + m_files.size(); }
bool Node::empty() { return size() == 0; }

std::string Node::abs_path() const
{
    return m_parent == nullptr ? m_inode->name()  : m_parent->abs_path() + "/" + m_inode->name();
}

bool Node::operator==(const Node& other) const
{
    return abs_path() == other.abs_path();
}

bool Node::operator<(const Node& other) const
{
    return abs_path() < other.abs_path();
}

bool Node::operator>(const Node& other) const
{
    return abs_path() > other.abs_path();
}

bool Node::operator>=(const Node& other) const
{
    return abs_path() >= other.abs_path();
}

bool Node::operator<=(const Node& other) const
{
    return abs_path() <= other.abs_path();
}

bool Node::operator!=(const Node& other) const
{
    return abs_path() != other.abs_path();
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

void Node::remove()
{
    utils::Ordered_list<Node*>::Link *link = nullptr;
    if (m_parent)
    {
        if (m_inode->is_directory())
            link = m_parent->m_dirs.unlink(this);
        else
            link = m_parent->m_files.unlink(this);
    }

    load();

    utils::Ordered_list<Node*>::Link *head = m_dirs.head();
    while (head)
    {
        auto tmp = head->next();
        head->data()->remove();
        head = tmp;
    }

    head = m_files.head();
    while (head)
    {
        auto tmp = head->next();
        head->data()->remove();
        head = tmp;
    }

    m_inode->remove(this);
    m_parent = nullptr;
    if (link && link->data())
        delete link->data();
    if (link)
        delete link;
}

}
