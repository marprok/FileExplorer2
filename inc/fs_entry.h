#ifndef FS_ENTRY
#define FS_ENTRY
#include <string>

namespace fs {

class FS_Entry
{
protected:
    std::string   m_name;
    FS_Entry*  m_parent;
    std::size_t   m_size;
    bool        m_loaded;
public:
    /**
     * @brief load loads the file system entry
     * @return the number of elements contained, if any
     */
    virtual std::size_t load() = 0;
    /**
     * @brief unload deallocates all resources of the file system entry object
     */
    virtual void unload() = 0;
    /**
     * @brief reload force reloads the file system entry
     * @return the number of elements contained, if any
     */
    std::size_t reload()
    {
        unload();
        return load();
    }
    /**
     * @brief abs_path
     * @return the name relative to the parent
     */
    std::string abs_path() const
    {
        return m_parent == nullptr ? m_name : m_parent->abs_path() + "/" + m_name;
    }
    /**
     * @brief name
     * @return the name if the file system entry
     */
    const std::string name() const
    {
        return m_name;
    }
    /**
     * @return loaded returns true if the entry has been succesfuly loaded
     */
    bool loaded() const
    {
        return m_loaded;
    }
    /**
     * @brief The destructor
     */
    virtual ~FS_Entry()
    {

    }

    FS_Entry(const std::string& name, FS_Entry* parent)
        :m_name(name), m_parent(parent), m_size(0), m_loaded(false)
    {

    }
};

}
#endif
