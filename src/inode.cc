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
    Inode::Inode(const std::string& abs_path)
        :m_path(abs_path)
    {
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

    void Inode::stat()
    {
        struct stat sstat;
        assert(!lstat(m_path.string().c_str(), &sstat));
        _rights(sstat);

        m_name = m_path.filename();
        m_mode = sstat.st_mode;
        m_last_accessed = _get_time(&sstat.st_atim);
        m_last_modified = _get_time(&sstat.st_mtim);
        m_last_changed = _get_time(&sstat.st_ctim);
        m_dev_num = std::to_string(sstat.st_dev);
        m_inode_num = std::to_string(sstat.st_ino);
        m_hard_links = std::to_string(sstat.st_nlink);
        m_size = std::to_string(sstat.st_size);
        m_uid = std::to_string(sstat.st_uid);
        m_gid = std::to_string(sstat.st_gid);
        m_dev_id = std::to_string(sstat.st_rdev);
        m_block_size = std::to_string(sstat.st_blksize);
        m_blocks = std::to_string(sstat.st_blocks);

        if (!is_directory())
            _format_size(sstat);

        if (is_symbolic_link())
            _compute_real_name(sstat);
    }

    const std::string& Inode::device_number() const
    {
        return m_dev_num;
    }

    const std::string& Inode::inode_number() const
    {
        return m_inode_num;
    }

    bool Inode::is_socket() const
    {
        return S_ISSOCK(m_mode);
    }

    bool Inode::is_symbolic_link() const
    {
        return S_ISLNK(m_mode);
    }

    bool Inode::is_regular_file() const
    {
        return S_ISREG(m_mode);
    }

    bool Inode::Inode::is_block_device() const
    {
        return S_ISBLK(m_mode);
    }

    bool Inode::is_directory() const
    {
        return S_ISDIR(m_mode);
    }

    bool Inode::is_character_device() const
    {
        return S_ISCHR(m_mode);
    }

    bool Inode::is_executable() const
    {
        return (m_mode & S_IXUSR)
               && (m_mode & S_IXGRP)
               && (m_mode & S_IXOTH);
    }


    const std::string& Inode::hard_link_count() const
    {
        return m_hard_links;
    }

    const std::string& Inode::size() const
    {
        return m_size;
    }

    const std::string& Inode::uid() const
    {
        return m_uid;
    }

    const std::string& Inode::gid() const
    {
        return m_gid;
    }

    const std::string& Inode::device_id() const
    {
        return m_dev_id;
    }

    const std::string& Inode::block_size() const
    {
        return m_block_size;
    }

    const std::string& Inode::blocks521B_allocated() const
    {
        return m_blocks;
    }

    const std::string& Inode::last_accessed() const
    {
        return m_last_accessed;
    }

    const std::string& Inode::last_modified() const
    {
        return m_last_modified;
    }

    const std::string& Inode::last_status_changed() const
    {
        return m_last_changed;
    }

    void Inode::_rights(const struct stat& sstat)
    {
        std::string temp;
        /* Owner */
        if (sstat.st_mode & S_IRUSR)
            m_rights += "r";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IWUSR)
            m_rights += "w";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IXUSR)
            m_rights += "x";
        else
            m_rights += "-";

        /* Group */
        if (sstat.st_mode & S_IRGRP)
            m_rights += "r";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IWGRP)
            m_rights += "w";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IXGRP)
            m_rights += "x";
        else
            m_rights += "-";

        /* Others */
        if (sstat.st_mode & S_IROTH)
            m_rights += "r";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IWOTH)
            m_rights += "w";
        else
            m_rights += "-";

        if (sstat.st_mode & S_IXOTH)
            m_rights += "x";
        else
            m_rights += "-";
    }

    const std::string& Inode::rights() const { return m_rights; }

    void Inode::_compute_real_name(const struct stat& sstat)
    {
        if (!is_symbolic_link())
            return;
        /* The following code is taken from the realink man page. */
        std::size_t size = static_cast<std::size_t>(sstat.st_size);

        if (size == 0)
            size = PATH_MAX;
        size++;
        char *buf = new char[size];
        memset(buf, 0, size);
        assert(readlink(m_path.string().c_str(), buf, size) != 0);
        m_real_name = buf; /* Deep copy */
        delete[] buf;
    }

    const std::string& Inode::real_name() const { return m_real_name; }

    const std::string& Inode::name() const
    {
        return m_name;
    }

    std::string Inode::abs_path() const
    {
        return m_path;
    }

    void Inode::_format_size(const struct stat& sstat)
    {
        std::array<long, 3> sizes = {1024l*1024l*1024l, 1024*1024l, 1024l};
        std::array<std::string, 3> names = {"GB", "MB", "KB"};
        m_formated_size = std::to_string(sstat.st_size) + "B";
        for (std::size_t i = 0; i < sizes.size(); ++i)
        {
            if (sstat.st_size / sizes[i])
            {
                m_formated_size = std::to_string(sstat.st_size / sizes[i]) + names[i];
                break;
            }
        }
    }

    const std::string& Inode::formated_size() const { return m_formated_size; }

    std::string Inode::parent() const { return m_path.parent_path(); }

    Inode Inode::parent_node() const
    {
        return {m_path.parent_path().string()};
    }

    std::size_t Inode::load(std::vector<Inode> &files, std::vector<Inode> &dirs) const
    {
        if (!is_directory())
            return 0;

        DIR *dir = opendir(m_path.string().c_str());
        if (!dir)
            return 0;
        struct dirent *drt;
        while ((drt = readdir(dir)))
        {
            if (drt->d_type != DT_DIR)
            {
                /* This is not a directory. */
                files.emplace_back(m_path / drt->d_name);
            }else
            {
                /* In case the m_name is . or .. */
                size_t temp = strlen(drt->d_name);
                if ((temp == 1 && drt->d_name[0] == '.') ||
                    (temp == 2 && drt->d_name[0] == '.' && drt->d_name[1] == '.'))
                {
                    continue;
                }
                dirs.emplace_back(m_path / drt->d_name);
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
            execlp("mv", "mv", m_path.string().c_str(), new_parent.m_path.string().c_str(), (char*)NULL);
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
            execlp("rm", "rm", "-r", "-f", m_path.string().c_str(), (char*)NULL);
            _exit(127);
        }

        assert(pid != -1);
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    bool Inode::copy(const Inode& new_parent) const
    {
        const std::string new_path = new_parent.m_path / m_name;
        pid_t pid = fork();
        if (!pid)
        {
            int nothingness = open("/dev/null",O_WRONLY);
            if (nothingness < 0)
                _exit(127);
            dup2(nothingness, 1);
            dup2(nothingness, 2);
            close(nothingness);
            execlp("cp", "cp", "-r", m_path.string().c_str(), new_path.c_str(), (char*)NULL);
            _exit(127);
        }

        assert(pid != -1);
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    bool Inode::create_dir(const std::string &name) const
    {
        auto full_path = m_path / name;
        return mkdir(full_path.c_str(), 0755) != -1;
    }

    bool Inode::create_file(const std::string &name) const
    {
        auto full_path = m_path / name;
        return creat(full_path.c_str(), 0644) != -1;
    }
}
