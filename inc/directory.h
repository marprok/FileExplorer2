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

    Directory(const Directory&) = delete;

    Directory& operator=(const Directory&) = delete;

    Directory& operator=(Directory&&) = default;

    Directory(Directory&&) = default;

    ~Directory() override;

    std::size_t populate(Node* node) override;
    int remove(const Node *node) override;
};

}
#endif // DIRECTORY_H
