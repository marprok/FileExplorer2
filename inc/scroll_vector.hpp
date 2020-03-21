#ifndef SCROLL_VECTOR_H
#define SCROLL_VECTOR_H

#include <vector>
#include <cassert>

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
    std::size_t m_begin_selection_index;
    std::size_t m_end_selection_index;
    bool m_selection_in_progress;
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
     * @param index the index of the element to be accessed[0, m_window_size)
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
     * @brief selection_in_progress
     * @return true if a selection is in progress, false otherwise
     */
    bool selection_in_progress() const;
    /**
     * @brief start_selection initiates a selection from the current index
     */
    void start_selection();
    /**
     * @brief selection_append inserts the current index in the selection
     */
    void selection_append();
    /**
     * @brief interupt_selection abruptly interupts the selection
     */
    void interupt_selection();
    /**
     * @brief is_selected
     * @param i the query index in window coordinates[0, m_window_size)
     * @return true if i is contained inside the selection
     */
    bool is_selected(std::size_t i) const;
    /**
     * @brief end_selection graceful termination of the selection
     * @return a vector containing the selected items
     */
    std::vector<T> end_selection();
    /**
     * @brief index returns the value of the internal window index[0, m_window_size)
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
      m_index(0),
      m_begin_selection_index(0),
      m_end_selection_index(0),
      m_selection_in_progress(false)
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
void scrollable_vector<T>::move_down()
{
    if (m_index < m_window_size - 1)
        m_index++;
    else
        scroll_down();
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
bool scrollable_vector<T>::selection_in_progress() const
{
    return m_selection_in_progress;
}

template<typename T>
void scrollable_vector<T>::interupt_selection()
{
    m_selection_in_progress = false;
    m_end_selection_index = 0;
    m_begin_selection_index = 0;
}

template<typename T>
bool scrollable_vector<T>::is_selected(std::size_t i) const
{
    if (!selection_in_progress())
        return false;

    std::size_t candidate = real_index(i);
    assert(candidate < m_data.size());

    return m_begin_selection_index <= candidate && candidate < m_end_selection_index;
}

template<typename T>
void scrollable_vector<T>::start_selection()
{
    if (m_data.empty())
        return;

    if (selection_in_progress())
        interupt_selection();

    m_selection_in_progress = true;
    m_begin_selection_index = real_index(index());
    assert(m_begin_selection_index < m_data.size());
    m_end_selection_index = m_begin_selection_index + 1;
}

template<typename T>
void scrollable_vector<T>::selection_append()
{
    if (!selection_in_progress())
        return;

    std::size_t canditate = real_index(index());
    assert(canditate < m_data.size());

    if (canditate < m_begin_selection_index)
        m_begin_selection_index = canditate;
    else if (canditate >= m_end_selection_index)
        m_end_selection_index++;
}

template<typename T>
std::vector<T> scrollable_vector<T>::end_selection()
{
    if (!selection_in_progress())
        return {};

    assert(m_begin_selection_index < m_end_selection_index);

    std::vector<T> selection;
    for (; m_begin_selection_index < m_end_selection_index; ++m_begin_selection_index)
        selection.push_back(m_data[m_begin_selection_index]);

    interupt_selection();

    return selection;
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
    interupt_selection();
}
}

#endif
