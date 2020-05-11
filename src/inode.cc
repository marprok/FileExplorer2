#include <array>
#include <vector>
#include <dirent.h>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../inc/inode.h"

namespace fs
{
    Inode::Inode(const std::string& parent, const std::string& name)
        :m_name(name), m_parent(parent)
    {
        // TODO: what if parent is a hiden direcotiry?
        if (strstr(m_parent.c_str(), ".") || strstr(m_parent.c_str(), ".."))
        {
            char full_path[PATH_MAX] = {0};
            assert(realpath(m_parent.c_str(), full_path));
            m_parent = full_path;
        }
        stat();
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

    int Inode::stat()
    {
        assert(!lstat(abs_path().c_str(), &m_stat));
        _rights();
        if (!is_directory())
            _format_size();
        if (is_symbolic_link())
            _compute_real_name();

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

    bool Inode::is_executable() const
    {
        return (m_stat.st_mode & S_IXUSR)
               && (m_stat.st_mode & S_IXGRP)
               && (m_stat.st_mode & S_IXOTH);
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

    void Inode::_compute_real_name()
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
        assert(readlink(abs_path().c_str(), buf, size) != 0);
        m_real_name = buf; /* Deep copy */
        delete[] buf;
    }

    std::string Inode::real_name() const { return m_real_name; }

    std::string Inode::name() const
    {
        return m_name;
    }

    std::string Inode::abs_path() const
    {
        if (m_parent == "")
            return "/";
        else if (m_parent == "/")
            return m_parent + m_name;

        return m_parent + "/" + m_name;
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

    std::string Inode::parent() const { return m_parent; }

    Inode Inode::parent_node() const
    {
        if (m_parent == "/" || m_parent == "")
            return {"", "/"};

        auto index = m_parent.rfind("/");
        assert(index != std::string::npos);
        if (index == 0)
            return {"/", m_parent.substr(1)};
        index++;
        return {m_parent.substr(0, index-1), m_parent.substr(index)};
    }

    std::size_t Inode::load(std::vector<Inode> &files, std::vector<Inode> &dirs) const
    {
        if (!is_directory())
            return 0;

        DIR *dir = opendir(abs_path().c_str());
        if (!dir)
            return 0;
        struct dirent *drt;
        // TODO: links to directories must go to dirs
        while ((drt = readdir(dir)))
        {
            if (drt->d_type != DT_DIR)
            {
                /* This is not a directory. */
                files.emplace_back(abs_path(), drt->d_name);
            }else
            {
                /* In case the m_name is . or .. */
                size_t temp = strlen(drt->d_name);
                if ((temp == 1 && drt->d_name[0] == '.') ||
                    (temp == 2 && drt->d_name[0] == '.' && drt->d_name[1] == '.'))
                {
                    continue;
                }
                dirs.emplace_back(abs_path(), drt->d_name);
            }
        }

        closedir(dir);
        return files.size() + dirs.size();
    }

    bool Inode::move(const Inode& new_parent) const
    {
        pid_t pid = fork();
        if (!pid)
        {
            int nothingness = open("/dev/null",O_WRONLY);
            if (nothingness < 0)
                _exit(127);
            dup2(nothingness, 1);
            dup2(nothingness, 2);
            close(nothingness);
            execlp("mv", "mv", abs_path().c_str(), new_parent.abs_path().c_str(), (char*)NULL);
            _exit(127);
        }

        assert(pid != -1);
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    bool Inode::remove() const
    {
        pid_t pid = fork();
        if (!pid)
        {
            int nothingness = open("/dev/null",O_WRONLY);
            if (nothingness < 0)
                _exit(127);
            dup2(nothingness, 1);
            dup2(nothingness, 2);
            close(nothingness);
            execlp("rm", "rm", "-r", "-f", abs_path().c_str(), (char*)NULL);
            _exit(127);
        }

        assert(pid != -1);
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    bool Inode::copy(const Inode& new_parent) const
    {
        const std::string new_path = new_parent.abs_path() + "/" + m_name;
        pid_t pid = fork();
        if (!pid)
        {
            int nothingness = open("/dev/null",O_WRONLY);
            if (nothingness < 0)
                _exit(127);
            dup2(nothingness, 1);
            dup2(nothingness, 2);
            close(nothingness);
            execlp("cp", "cp", "-r", abs_path().c_str(), new_path.c_str(), (char*)NULL);
            _exit(127);
        }

        assert(pid != -1);
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    bool Inode::create_dir(const std::string &name) const
    {
        auto full_path = abs_path() + "/" + name;
        return mkdir(full_path.c_str(), 0755) != -1;
    }

    bool Inode::create_file(const std::string &name) const
    {
        auto full_path = abs_path() + "/" + name;
        return creat(full_path.c_str(), 0644) != -1;
    }
}
