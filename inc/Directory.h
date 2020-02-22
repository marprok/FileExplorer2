#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <vector>
#include <string>
#include <memory>
#include "File.h"
#include "fs_entry.h"

namespace fs
{
/**
 * @brief The Directory class
 *  This class represents a directory
 *  in the file system.
 */
class Directory : public FS_Entry
{
private:
    std::size_t                m_index;
    std::vector<File>          m_files;
    std::vector<Directory*>    m_dirs;
public:
    /* FS entry API */
    virtual std::size_t load() override;

    virtual void unload() override;

    Directory(const std::string& name, FS_Entry* parent);
    /**
     * @brief The destructor deletes all the Directory pointers.
     */
    ~Directory();
    /**
     * @brief dive access a child directory
     * @param i
     * @return  the child directory
     */
    Directory* dive(std::size_t i);
    /**
     * @brief surface return to the parent directory
     * @return the parent directory
     */
    Directory* surface();

    std::size_t index() const;
    std::vector<File>& files();
    std::vector<Directory*>& dirs();
    bool empty() const;
    /**
     * @brief create_file
     * @param name
     * @return a file descroptor in case of success
     */
    int create_file(const std::string& name);
    /**
     * @brief unlink_file
     * @param i
     * @return zero in case of success
     */
    int unlink_file(std::size_t i);
};
}
#endif // DIRECTORY_H
