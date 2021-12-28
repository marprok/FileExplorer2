#ifndef INODE_H
#define INODE_H

#include <sys/stat.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <cassert>
#include <filesystem>

namespace sfs = std::filesystem;

namespace fs {
    class Inode
    {
        sfs::path m_path;
        std::string m_name;
        std::string m_rights;
        std::string m_real_name;
        std::string m_formated_size;
        std::string m_last_accessed;
        std::string m_last_modified;
        std::string m_last_changed;
        std::string m_dev_num;
        std::string m_inode_num;
        std::string m_hard_links;
        std::string m_size;
        std::string m_uid;
        std::string m_gid;
        std::string m_dev_id;
        std::string m_block_size;
        mode_t m_mode;
        std::string m_blocks;


    public:

        Inode(const std::string& parent);

        Inode(const Inode& other) = default;

        Inode& operator=(const Inode& other) = default;

        Inode(Inode&& other) = default;

        Inode& operator=(Inode&& other) = default;

        ~Inode();

        void stat();

        // INode data access
        const std::string& device_number() const;

        const std::string& inode_number() const;

        bool is_socket() const;

        bool is_symbolic_link() const;

        bool is_regular_file() const;

        bool is_block_device() const;

        bool is_directory() const;

        bool is_character_device() const;

        bool is_executable() const;

        const std::string& hard_link_count() const;

        const std::string& size() const;

        const std::string& uid() const;

        const std::string& gid() const;

        const std::string& device_id() const;

        const std::string& block_size() const;

        const std::string& blocks521B_allocated() const;

        // Timestamps
        const std::string& last_accessed() const;

        const std::string& last_modified() const;

        const std::string& last_status_changed() const;

        // Utility methods
        const std::string& rights() const;

        const std::string& real_name() const;

        const std::string& name() const;

        std::string abs_path() const;

        const std::string& formated_size() const;

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
        void _rights(const struct stat& sstat);
        void _format_size(const struct stat& sstat);
        void _compute_real_name(const struct stat& sstat);
    };

}
#endif // INODE_H
