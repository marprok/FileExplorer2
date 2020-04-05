#ifndef FILE_H
#define FILE_H
#include <string>
#include "inode.h"

namespace fs
{
class File : public Inode
{
public:
    File(const std::string& name);
    ~File();

    std::size_t populate(Node* node) override;
    int remove(const Node *node) override;
};
}

#endif //FILE_H
