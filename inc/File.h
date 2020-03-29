#ifndef FILE_H
#define FILE_H
#include <string>
#include "fs_data.h"

namespace fs
{
class File : public Inode
{
public:
    File(const std::string& name);

    std::size_t populate(FS_Node* node) override;
    void copy(Inode* new_parent) override;
    void move(Inode* new_parent) override;
};
}

#endif //FILE_H
