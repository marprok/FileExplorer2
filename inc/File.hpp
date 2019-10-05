#ifndef FILE_H
#define FILE_H

#include <sys/stat.h>
#include <string>

namespace fs
{
    class File
    {
    private:
        struct stat m_stat;
        std::string m_name;
        std::string m_parent_name;

        int _read_data();
        std::string _get_time(const struct timespec* tp) const;
    public:
        File(const std::string &name, const std::string &parent_name);
        std::string abs_path() const;
        const std::string& name() const;

        /* INode data access */
        std::string inode_number() const;
        std::string hlinks_number() const;
        std::string size() const;
        std::string last_mod() const;
        std::string last_acc() const;
        std::string rights() const;
        bool is_link() const;
    };
}

#endif //FILE_H
