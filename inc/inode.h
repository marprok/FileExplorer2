#ifndef INODE_H
#define INODE_H

#include <sys/stat.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <cassert>

namespace fs {
    class Inode
    {
    private:

        class Absolute_path
        {
        private:
            std::string m_path;

        public:

            Absolute_path(const std::string &path);

            std::string parent_part() const;

            std::string name_part() const;

            const std::string& path() const;

            std::string append(const std::string &part) const;

        };

        Absolute_path m_abs_path;
        std::string m_name;
        std::string m_rights;
        std::string m_real_name;
        std::string m_formated_size;
        struct stat m_stat;

    public:

        Inode(const std::string& parent);

        Inode(const Inode& other) = default;

        Inode& operator=(const Inode& other) = default;

        Inode(Inode&& other) = default;

        Inode& operator=(Inode&& other) = default;

        ~Inode();

        int stat();

        // INode data access
        std::string device_number() const;

        std::string inode_number() const;

        bool is_socket() const;

        bool is_symbolic_link() const;

        bool is_regular_file() const;

        bool is_block_device() const;

        bool is_directory() const;

        bool is_character_device() const;

        bool is_executable() const;

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

        // Utility methods
        std::string rights() const;

        std::string real_name() const;

        std::string name() const;

        std::string abs_path() const;

        std::string formated_size() const;

        std::string parent() const;

        Inode parent_node() const;

        std::size_t load(std::vector<Inode> &files, std::vector<Inode> &dirs) const;

        bool move(const Inode& new_parent) const;

        bool remove() const;

        bool copy(const Inode& new_parent) const;

        bool create_dir(const std::string &name) const;

        bool create_file(const std::string &name) const;

    private:
        std::string _get_time(const struct timespec* tp) const;
        void _rights();
        void _format_size();
        void _compute_real_name();
    };

}
#endif // INODE_H
