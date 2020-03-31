#include <cstdio> // rename
#include "../inc/fs_node.h"

namespace fs
{

FS_Node::FS_Node(Inode* inode, FS_Node* parent)
    :m_parent(parent), m_inode(inode),m_loaded(false)
{
    m_inode->stat(abs_path());
}

FS_Node::~FS_Node()
{
    if (m_inode)
        delete m_inode;
}

std::size_t FS_Node::load()
{
    if (m_loaded)
        return size();
    m_loaded = true;
    return m_inode->populate(this);
}

void FS_Node::copy(FS_Node *new_parent)
{
    (void)new_parent;
}

void FS_Node::move(FS_Node *new_parent)
{
    if (!new_parent)
        return;
    // TODO: check if this is a parent of the new parent
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

}
