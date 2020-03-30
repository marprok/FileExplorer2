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

class FS_Node;

class Inode
{
private:
    std::string _get_time(const struct timespec* tp) const
    {
        char buf[64];
        struct tm* t = localtime(&tp->tv_sec);
        if (!t)
            return "";
        strftime(buf, sizeof (buf),  "%b %d %H:%M", t);
        return buf;
    }
protected:
    std::string m_name;
    std::string m_rights;
    std::string m_real_name;
    struct stat m_stat;
public:

    Inode(const std::string& name)
        :m_name(name)
    {

    }

    virtual ~Inode()
    {

    }

    inline int stat(const std::string& abs_path)
    {
        return lstat(abs_path.c_str(), &m_stat);
    }

    //INode data access
    inline std::string device_number() const
    {
        return std::to_string(m_stat.st_dev);
    }

    inline std::string inode_number() const
    {
        return std::to_string(m_stat.st_ino);
    }
    //file type and mode
    inline bool is_socket() const
    {
        return S_ISSOCK(m_stat.st_mode);
    }

    inline bool is_symbolic_link() const
    {
        return S_ISLNK(m_stat.st_mode);
    }

    inline bool is_regular_file() const
    {
        return S_ISREG(m_stat.st_mode);
    }

    inline bool is_block_device() const
    {
        return S_ISBLK(m_stat.st_mode);
    }

    inline bool is_directory() const
    {
        return S_ISDIR(m_stat.st_mode);
    }

    inline bool is_character_device() const
    {
        return S_ISCHR(m_stat.st_mode);
    }

    inline std::string hard_link_count() const
    {
        return std::to_string(m_stat.st_nlink);
    }

    inline std::string size() const
    {
        return std::to_string(m_stat.st_size);
    }

    inline std::string uid() const
    {
        return std::to_string(m_stat.st_uid);
    }

    inline std::string guid() const
    {
        return std::to_string(m_stat.st_gid);
    }

    inline std::string device_id() const
    {
        return std::to_string(m_stat.st_rdev);
    }

    inline std::string block_size() const
    {
        return std::to_string(m_stat.st_blksize);
    }

    inline std::string blocks521B_allocated() const
    {
        return std::to_string(m_stat.st_blocks);
    }

    // Timestamps
    inline std::string last_accessed() const
    {
        return _get_time(&m_stat.st_atim);
    }

    inline std::string last_modified() const
    {
        return _get_time(&m_stat.st_mtim);
    }

    inline std::string last_status_changed() const
    {
        return _get_time(&m_stat.st_ctim);
    }

    // utility methods
    std::string rights()
    {
        if (!m_rights.empty())
            return m_rights;

        /* Owner */
        if (m_stat.st_mode & S_IRUSR)
            m_rights += "r";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IWUSR)
            m_rights += "w";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IXUSR)
            m_rights += "x";
        else
            m_rights += "-";

        /* Group */
        if (m_stat.st_mode & S_IRGRP)
            m_rights += "r";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IWGRP)
            m_rights += "w";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IXGRP)
            m_rights += "x";
        else
            m_rights += "-";

        /* Others */
        if (m_stat.st_mode & S_IROTH)
            m_rights += "r";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IWOTH)
            m_rights += "w";
        else
            m_rights += "-";

        if (m_stat.st_mode & S_IXOTH)
            m_rights += "x";
        else
            m_rights += "-";

        return m_rights;
    }

    std::string real_name(const std::string& abs_path)
    {
        if (!m_real_name.empty())
            return m_real_name;

        if (!is_symbolic_link())
            return m_real_name;
        /* The following code is taken from the realink man page. */
        std::size_t size = m_stat.st_size;

        if (size == 0)
        {
            size = PATH_MAX;
        }
        size++;
        char *buf = new char[size];
        memset(buf, 0, size);

        if (readlink(abs_path.c_str(), buf, size) < 0)
        {
            std::cerr << "Cannot readlink:" << name() << std::endl;
            return "";
        }
        m_real_name = buf; /* Deep copy */
        delete[] buf;
        return m_real_name;
    }

    inline std::string name() const
    {
        return m_name;
    }
    // Abstract API
    virtual std::size_t populate(FS_Node* node) = 0;
    virtual void copy(Inode* new_parent) = 0;
    virtual void move(Inode* new_parent) = 0;
};

}
#endif
