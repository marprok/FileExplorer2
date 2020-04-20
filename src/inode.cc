#include "../inc/inode.h"
#include <array>

namespace fs
{
    Inode::Inode(const std::string& name)
        :m_name(name)
    {

    }

    Inode::~Inode()
    {

    }

    std::string Inode::_get_time(const struct timespec* tp) const
    {
        char buf[64];
        struct tm* t = localtime(&tp->tv_sec);
        if (!t)
            return "";
        strftime(buf, sizeof (buf),  "%b %d %H:%M", t);
        return buf;
    }

    int Inode::stat(const std::string& abs_path)
    {
        return lstat(abs_path.c_str(), &m_stat);
    }

    std::string Inode::device_number() const
    {
        return std::to_string(m_stat.st_dev);
    }

    std::string Inode::inode_number() const
    {
        return std::to_string(m_stat.st_ino);
    }

    bool Inode::is_socket() const
    {
        return S_ISSOCK(m_stat.st_mode);
    }

    bool Inode::is_symbolic_link() const
    {
        return S_ISLNK(m_stat.st_mode);
    }

    bool Inode::is_regular_file() const
    {
        return S_ISREG(m_stat.st_mode);
    }

    bool Inode::Inode::is_block_device() const
    {
        return S_ISBLK(m_stat.st_mode);
    }

    bool Inode::is_directory() const
    {
        return S_ISDIR(m_stat.st_mode);
    }

    bool Inode::is_character_device() const
    {
        return S_ISCHR(m_stat.st_mode);
    }

    std::string Inode::hard_link_count() const
    {
        return std::to_string(m_stat.st_nlink);
    }

    std::string Inode::size() const
    {
        return std::to_string(m_stat.st_size);
    }

    std::string Inode::uid() const
    {
        return std::to_string(m_stat.st_uid);
    }

    std::string Inode::guid() const
    {
        return std::to_string(m_stat.st_gid);
    }

    std::string Inode::device_id() const
    {
        return std::to_string(m_stat.st_rdev);
    }

    std::string Inode::block_size() const
    {
        return std::to_string(m_stat.st_blksize);
    }

    std::string Inode::blocks521B_allocated() const
    {
        return std::to_string(m_stat.st_blocks);
    }

    std::string Inode::last_accessed() const
    {
        return _get_time(&m_stat.st_atim);
    }

    std::string Inode::last_modified() const
    {
        return _get_time(&m_stat.st_mtim);
    }

    std::string Inode::last_status_changed() const
    {
        return _get_time(&m_stat.st_ctim);
    }

    std::string Inode::rights()
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

    std::string Inode::real_name(const std::string& abs_path)
    {
        if (!m_real_name.empty())
            return m_real_name;

        if (!is_symbolic_link())
            return m_real_name;
        /* The following code is taken from the realink man page. */
        std::size_t size = static_cast<std::size_t>(m_stat.st_size);

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

    std::string Inode::name() const
    {
        return m_name;
    }

    std::string Inode::formated_size()
    {
        if (!m_formated_size.empty())
            return m_formated_size;

        std::array<long, 3> sizes = {1024l*1024l*1024l, 1024*1024l, 1024l};
        std::array<std::string, 3> names = {"GB", "MB", "KB"};
        m_formated_size = std::to_string(m_stat.st_size) + "B";
        for (std::size_t i = 0; i < sizes.size(); ++i)
        {
            if (m_stat.st_size / sizes[i])
            {
                m_formated_size = std::to_string(m_stat.st_size / sizes[i]) + names[i];
                break;
            }
        }

        return m_formated_size;
    }
}
