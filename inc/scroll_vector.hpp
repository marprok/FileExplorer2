#ifndef SCROLL_VECTOR_H
#define SCROLL_VECTOR_H

#include <vector>

namespace utils
{
/**
 * @brief A vector that supports a sliding window.
 *  Given a specific sliding window size, the vector
 *  can scroll down/up and access the values that are
 *  visible to the current window.
 */
template<typename T>
class scrollable_vector
{
private:
    std::vector<T> m_data;
    std::size_t m_base;
    std::size_t m_window_size;
    std::size_t m_limit;
    std::size_t m_index;
public:
    /**
     * @brief ScrollableVector
     * @param base the index of the first value of the window
     * @param window_size the sliding window size
     * @param data the vecotor that contains the data
     */
    scrollable_vector(std::size_t base, std::size_t window_size,
                     const std::vector<T> & data);
    scrollable_vector(const scrollable_vector& other) = default;
    scrollable_vector& operator=(const scrollable_vector& other) = default;
    /**
     * @brief operator []
     * @param index the index of the element to be accessed(0, m_window_size)
     * @return A reference to the element
     */
    T& operator[](std::size_t index);
    /**
     * @brief real_index
     * @param index of an element inside the window
     * @return the real index of the element in the data vector
     */
    std::size_t real_index(std::size_t index) const;
    /**
     * @brief can_scroll_up
     * @return true if sroll up is allowed
     */
    bool can_scroll_up() const;
    /**
     * @brief can_scroll_down
     * @return true if scroll down is allowed
     */
    bool can_scroll_down() const;
    /**
     * @brief scroll_up scroll up the vector
     */
    void scroll_up();
    /**
     * @brief scroll_down scroll down the vector
     */
    void scroll_down();
    /**
     * @brief move_down moves the index down by one and scrolls down if necessary
     */
    void move_down();
    /**
     * @brief move_up moves the index up by one and scrolls up if necessary
     */
    void move_up();
    /**
     * @brief index returns the value of the internal window index(0, m_window_size -1)
     * @return the index
     */
    std::size_t index() const;
    /**
     * @brief size
     * @return the size of the vector
     */
    std::size_t size() const;
    /**
     * @brief reset reset the state of the object
     * @param base
     * @param window_size
     * @param data
     */
    void reset(std::size_t base, std::size_t window_size,
               std::vector<T> & data);
};

template<typename T>
scrollable_vector<T>::scrollable_vector(std::size_t base, std::size_t window_size,
                                      const std::vector<T> & data)
    :m_data(data),
      m_base(base),
      m_window_size(window_size),
      m_limit(m_base + m_window_size),
      m_index(0)
{
    if (m_window_size > m_data.size())
    {
        m_window_size = m_data.size();
        m_limit = m_base + m_window_size;
    }
}

template<typename T>
T& scrollable_vector<T>::operator[](std::size_t index)
{
    /* No bounds checking */
    return m_data[m_base + index];
}

template<typename T>
bool scrollable_vector<T>::can_scroll_down() const
{
    return m_limit < m_data.size();
}

template<typename T>
bool scrollable_vector<T>::can_scroll_up() const
{
    return m_base > 0;
}

template<typename T>
void scrollable_vector<T>::scroll_up()
{
    if (can_scroll_up())
    {
        m_base--;
        m_limit--;
    }
}

template<typename T>
void scrollable_vector<T>::scroll_down()
{
    if (can_scroll_down())
    {
        m_base++;
        m_limit++;
    }
}

template<typename T>
void scrollable_vector<T>::move_up()
{
    if (m_index > 0 )
        m_index--;
    else
        scroll_up();
}

template<typename T>
void scrollable_vector<T>::move_down()
{
    if (m_index < m_window_size - 1)
        m_index++;
    else
        scroll_down();
}

template<typename T>
std::size_t scrollable_vector<T>::real_index(std::size_t index) const
{
    return m_base + index;
}

template<typename T>
std::size_t scrollable_vector<T>::size() const
{
    return m_data.size();
}

template<typename T>
std::size_t scrollable_vector<T>::index() const
{
    return m_index;
}

template<typename T>
void scrollable_vector<T>::reset(std::size_t base, std::size_t window_size,
                                std::vector<T> &data)
{
    m_data = data;
    m_base = base;
    m_window_size = window_size;
    m_limit = m_base + m_window_size;
    m_index = 0;
    if (m_window_size > m_data.size())
    {
        m_window_size = m_data.size();
        m_limit = m_base + m_window_size;
    }
}
}

#endif
