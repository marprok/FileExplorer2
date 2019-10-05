#include "../inc/File.hpp"
#include <time.h>
#include <iostream>

namespace fs
{
    File::File(const std::string &name, const std::string &parent_name)
        :m_name(name), m_parent_name(parent_name)
    {
        if (_read_data() < 0)
        {
            std::cerr << "Cannot open " << abs_path() << std::endl;
            std::exit(1);
        }
    }

    int File::_read_data()
    {
        int ret = 0;
        if ((ret = lstat(abs_path().c_str(), &m_stat))< 0)
            return ret;
        return 0;
    }

    std::string File::abs_path() const
    {
        return m_parent_name + "/" + m_name;
    }

    const std::string& File::name() const
    {
        return m_name;
    }

    std::string File::inode_number() const
    {
        return std::to_string(m_stat.st_ino);
    }

    std::string File::hlinks_number() const
    {
        return std::to_string(m_stat.st_nlink);
    }

    std::string File::size() const
    {
        return std::to_string(m_stat.st_size);
    }

    std::string File::last_mod() const
    {
        return _get_time(&m_stat.st_mtim);
    }

    std::string File::last_acc() const
    {
        return _get_time(&m_stat.st_atim);
    }

    std::string File::_get_time(const struct timespec* tp) const
    {
        char buf[64];
        struct tm* t = localtime(&tp->tv_sec);
        if (!t)
            return "";
        strftime(buf, sizeof (buf),  "%b %d %H:%M", t);
        return buf;
    }

    std::string File::rights() const
    {
        std::string out;
        /* Owner */
        if (m_stat.st_mode & S_IRUSR)
            out += "r";
        else
            out += "-";

        if (m_stat.st_mode & S_IWUSR)
            out += "w";
        else
            out += "-";

        if (m_stat.st_mode & S_IXUSR)
            out += "x";
        else
            out += "-";

        /* Group */
        if (m_stat.st_mode & S_IRGRP)
            out += "r";
        else
            out += "-";

        if (m_stat.st_mode & S_IWGRP)
            out += "w";
        else
            out += "-";

        if (m_stat.st_mode & S_IXGRP)
            out += "x";
        else
            out += "-";

        /* Others */
        if (m_stat.st_mode & S_IROTH)
            out += "r";
        else
            out += "-";

        if (m_stat.st_mode & S_IWOTH)
            out += "w";
        else
            out += "-";

        if (m_stat.st_mode & S_IXOTH)
            out += "x";
        else
            out += "-";

        return out;
    }

    bool File::is_link() const
    {
        return S_ISLNK(m_stat.st_mode);
    }
}
