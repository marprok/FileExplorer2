#ifndef INODE_LIST_HPP
#define INODE_LIST_HPP
#include <cstddef>

namespace fs
{
template<typename T>
class Linked_list
{
public:
    class Link
    {
      private:
        T* m_data;
        Link* m_next;
    public:
        Link(T* data, Link* next = nullptr)
            :m_data(data), m_next(next)
        {

        }

        T* data()
        {
            return m_data;
        }

        bool operator==(const Link& other)
        {
            return *m_data == *other.m_data;
        }

        Link* next() { return m_next; }

        void set_next(Link* next) { m_next = next; }

        ~Link()
        {
            if (m_data)
            {
                delete m_data;
            }
        }
    };

private:
    Link* m_head;
    std::size_t m_size;
public:
    Linked_list()
        :m_head(nullptr), m_size(0)
    {

    }

    ~Linked_list()
    {
        while(m_head)
        {
            auto tmp = m_head->next();
            delete m_head;
            m_head = tmp;
        }
        m_size = 0;
    }

    Link* head() { return m_head; }

    Link* get(T* data)
    {
        auto tmp = m_head;
        while (tmp)
        {
            if (*tmp->data() == *data)
                return tmp;
            tmp = tmp->next();
        }
        return nullptr;
    }

    bool insert_front(T* data)
    {
        if (get(data))
            return false;

        if (!m_head)
        {
            m_head = new Link(data);
            m_head->set_next(nullptr);
            m_size++;
            return true;
        }

        auto tmp = m_head;
        m_head = new Link(data);
        m_head->set_next(tmp);
        m_size++;
        return true;
    }

    void link(Link* node)
    {
        if (!node)
            return;
        node->set_next(m_head);
        m_head = node;
        m_size++;
    }

    std::size_t size() const { return m_size; }
    bool empty() { return m_head == nullptr; }

    Link* unlink(T* data)
    {
        if (empty() || !data)
            return nullptr;

        if (*m_head->data() == *data)
        {
            auto tmp = m_head;
            m_head = m_head->next();
            m_size--;
            return tmp;
        }

        Link* prev = nullptr;
        Link* temp = m_head;

        while (temp && !(*temp->data() == *data))
        {
            prev = temp;
            temp = temp->next();
        }

        if (*temp->data() == *data)
        {
            m_size--;
            prev->set_next(temp->next());
            temp->set_next(nullptr);
            return temp;
        }

        return nullptr;
    }

    void move(Linked_list& to, T* data)
    {
        auto tmp = unlink(data);
        if (tmp)
            to.link(tmp);
    }

    Link* copy(T* data)
    {
        auto tmp = get(data);

        return tmp;

    }
};
}
#endif //INODE_LIST_HPP
