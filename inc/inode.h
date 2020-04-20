#ifndef FS_ENTRY
#define FS_ENTRY

#include <sys/stat.h>
#include <time.h>
#include <iostream>
#include <linux/limits.h> // PATH_MAX
#include <unistd.h> // readlink
#include <cstring>
#include <string>
#include <cassert>

namespace fs {

class Node;

class Inode
{
private:
    std::string _get_time(const struct timespec* tp) const;
protected:
    std::string m_name;
    std::string m_rights;
    std::string m_real_name;
    std::string m_formated_size;
    struct stat m_stat;
public:

    Inode(const std::string& name);
    Inode(const Inode& other) = default;
    Inode& operator=(const Inode& other) = default;
    Inode(Inode&& other) = default;
    Inode& operator=(Inode&& other) = default;

    ~Inode();

    int stat(const std::string& abs_path);

    //INode data access
    std::string device_number() const;

    std::string inode_number() const;

    //file type and mode
    bool is_socket() const;

    bool is_symbolic_link() const;

    bool is_regular_file() const;

    bool is_block_device() const;

    bool is_directory() const;

    bool is_character_device() const;

    std::string hard_link_count() const;

    std::string size() const;

    std::string uid() const;

    std::string guid() const;

    std::string device_id() const;

    std::string block_size() const;

    std::string blocks521B_allocated() const;

    // Timestamps
    std::string last_accessed() const;

    std::string last_modified() const;

    std::string last_status_changed() const;

    // utility methods
    std::string rights();

    std::string real_name(const std::string& abs_path);

    std::string name() const;

    std::string formated_size();
};

}
#endif
