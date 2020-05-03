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
        if (lstat(abs_path.c_str(), &m_stat) < 0)
            return -1;

        _rights();
        _format_size();
        if (is_symbolic_link())
            _compute_real_name(abs_path);

        return 0;
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

    void Inode::_rights()
    {
        std::string temp;
        /* Owner */
        if (m_stat.st_mode & S_IRUSR)
            temp += "r";
        else
            temp += "-";

        if (m_stat.st_mode & S_IWUSR)
            temp += "w";
        else
            temp += "-";

        if (m_stat.st_mode & S_IXUSR)
            temp += "x";
        else
            temp += "-";

        /* Group */
        if (m_stat.st_mode & S_IRGRP)
            temp += "r";
        else
            temp += "-";

        if (m_stat.st_mode & S_IWGRP)
            temp += "w";
        else
            temp += "-";

        if (m_stat.st_mode & S_IXGRP)
            temp += "x";
        else
            temp += "-";

        /* Others */
        if (m_stat.st_mode & S_IROTH)
            temp += "r";
        else
            temp += "-";

        if (m_stat.st_mode & S_IWOTH)
            temp += "w";
        else
            temp += "-";

        if (m_stat.st_mode & S_IXOTH)
            temp += "x";
        else
            temp += "-";

        m_rights = temp;
    }

    std::string Inode::rights() const { return m_rights; }

    void Inode::_compute_real_name(const std::string& abs_path)
    {
        if (!is_symbolic_link())
            return;
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
            assert(1);
        }
        m_real_name = buf; /* Deep copy */
        delete[] buf;
    }

    std::string Inode::real_name() const { return m_real_name; }

    std::string Inode::name() const
    {
        return m_name;
    }

    void Inode::_format_size()
    {
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
    }

    std::string Inode::formated_size() const { return m_formated_size; }
}
