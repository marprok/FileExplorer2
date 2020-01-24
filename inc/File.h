#ifndef FILE_H
#define FILE_H

#include <sys/stat.h>
#include <string>

namespace fs
{
/**
 * @brief The File class
 *  This class represents a file
 *  in the file system. It contains
 *  most of the information that the stat
 *  system call returns.
 */
class File
{
private:
    struct stat m_stat;
    std::string m_name;
    std::string m_parent_name;
    bool m_loaded;
    /**
     * @brief _read_data reads the stat data
     * @return zero in case of success
     */
    int _read_data();
    /**
     * @brief _get_time formats the time
     * @param tp
     * @return  the formated time
     */
    std::string _get_time(const struct timespec* tp) const;
public:
    File(const std::string &name, const std::string &parent_name);
    /**
     * @brief abs_path
     * @return the path relative to the parent
     */
    std::string abs_path() const;
    /**
     * @brief name
     * @return name of the file
     */
    const std::string& name() const;
    /**
     * @brief loaded
     * @return true if already loaded
     */
    bool loaded() const;

    /* INode data access */
    std::string inode_number() const;
    std::string hlinks_number() const;
    std::string size() const;
    std::string last_mod() const;
    std::string last_acc() const;
    std::string rights() const;
    bool is_link() const;
    std::string real_file() const;
};
}

#endif //FILE_H
