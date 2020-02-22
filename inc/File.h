#ifndef FILE_H
#define FILE_H

#include <sys/stat.h>
#include <string>
#include "fs_entry.h"

namespace fs
{
/**
 * @brief The File class
 *  This class represents a file
 *  in the file system. It contains
 *  most of the information that the stat
 *  system call returns.
 */
class File : public FS_Entry
{
private:
    struct stat m_stat;
    /**
     * @brief _get_time formats the time
     * @param tp
     * @return  the formated time
     */
    std::string _get_time(const struct timespec* tp) const;
public:
    /* FS entry API */
    virtual std::size_t load() override;

    virtual void unload() override;

    File(const std::string &name, FS_Entry* parent);

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
