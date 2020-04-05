#include <unistd.h> // unlink
#include "../inc/file.h"
#include "../inc/node.h"

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

 int File::remove(const Node *node)
 {
     assert(node);
     return unlink(node->abs_path().c_str());
 }

}
