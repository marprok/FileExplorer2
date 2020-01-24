#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <vector>
#include <string>
#include <memory>
#include "File.h"

namespace fs
{
/**
 * @brief The Directory class
 *  This class represents a directory
 *  in the file system.
 */
class Directory
{
public:
private:
    std::string                m_name;
    Directory*                 m_parent;
    std::size_t                m_size;
    bool                       m_loaded;
    std::size_t                m_index;
    std::vector<File>          m_files;
    std::vector<Directory*>    m_dirs;
public:
    Directory(const std::string& name, Directory* parent);
    /**
     * @brief load_info loads the directory
     * @return the number of elements contained
     */
    std::size_t load_info();
    /**
     * @brief reload_info force reloads the directory
     * @return the size of the directory
     */
    std::size_t reload_info();
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
    /**
     * @brief abs_path
     * @return the name relative to the parent
     */
    std::string abs_path() const;
    /**
     * @brief name
     * @return the name if the directory
     */
    const std::string name() const;
    bool loaded() const;
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
