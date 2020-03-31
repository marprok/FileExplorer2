#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <vector>
#include <string>
#include <memory>
#include "inode.h"

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
    ~Directory();

    std::size_t populate(Node* node) override;
    void copy(Inode* new_parent) override;
    void move(Inode* new_parent) override;
};

}
#endif // DIRECTORY_H
