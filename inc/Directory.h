#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <vector>
#include <string>
#include <memory>
#include "fs_data.h"

namespace fs
{
/**
 * @brief The Directory class
 *  This class represents a directory
 *  in the file system.
 */
class Directory : public Inode
{
public:
    Directory(const std::string& name);

    std::size_t populate(FS_Node* node) override;
    void copy(Inode* new_parent) override;
    void move(Inode* new_parent) override;
};

}
#endif // DIRECTORY_H
