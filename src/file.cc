#include "../inc/file.h"

namespace fs
{

File::File(const std::string& name)
    :Inode (name)
{

}

File::~File()
{

}

std::size_t File::populate(Node* node)
{
    (void)node;
    // should not be called on a file node
    return 0;
}

void File::copy(Inode* new_parent)
{
    (void)new_parent;
}

void File::move(Inode* new_parent)
{
    (void)new_parent;
}

}
