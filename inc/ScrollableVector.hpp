#ifndef SCROLL_VECTOR_H
#define SCROLL_VECTOR_H

#include <vector>

namespace utils
{
    template<typename T>
    class SCRVector
    {
    private:
        std::vector<T> m_data;
        std::size_t m_base;
        std::size_t m_window_size;
        std::size_t m_limit;
    public:
        SCRVector(std::size_t base, std::size_t window_size,
                  std::vector<T> & data);
        SCRVector(const SCRVector& other) = default;
        SCRVector& operator=(const SCRVector& other) = default;
        T& operator[](size_t index);
        std::size_t real_index(std::size_t index);
        bool can_scroll_up();
        bool can_scroll_down();
        void scroll_up();
        void scroll_down();
        std::size_t size() const;
        void reset(std::size_t base, std::size_t window_size,
                   std::vector<T> & data);
    };

    template<typename T>
    SCRVector<T>::SCRVector(std::size_t base, std::size_t window_size,
                            std::vector<T> & data)
        :m_data(data),
         m_base(base),
         m_window_size(window_size),
         m_limit(m_base + m_window_size)
    {
        if (m_window_size > m_data.size())
        {
            m_window_size = m_data.size();
            m_limit = m_base + m_window_size;
        }
    }

    template<typename T>
    T& SCRVector<T>::operator[](size_t index)
    {
        /* No bounds checking */
        return m_data[m_base + index];
    }

    template<typename T>
    bool SCRVector<T>::can_scroll_down()
    {
        return m_limit < m_data.size();
    }

    template<typename T>
    bool SCRVector<T>::can_scroll_up()
    {
        return m_base > 0;
    }

    template<typename T>
    void SCRVector<T>::scroll_up()
    {
        if (can_scroll_up())
        {
            m_base--;
            m_limit--;
        }
    }

    template<typename T>
    void SCRVector<T>::scroll_down()
    {
        if (can_scroll_down())
        {
            m_base++;
            m_limit++;
        }
    }

    template<typename T>
    std::size_t SCRVector<T>::real_index(std::size_t index)
    {
        return m_base + index;
    }

    template<typename T>
    std::size_t SCRVector<T>::size() const
    {
        return m_data.size();
    }

    template<typename T>
    void SCRVector<T>::reset(std::size_t base, std::size_t window_size,
                             std::vector<T> &data)
    {
        m_data = data;
        m_base = base;
        m_window_size = window_size;
        m_limit = m_base + m_window_size;
        if (m_window_size > m_data.size())
        {
            m_window_size = m_data.size();
            m_limit = m_base + m_window_size;
        }
    }
}

#endif
