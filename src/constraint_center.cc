#include "../inc/constraint_center.h"

namespace utils {

Constraint_center::Constraint_center( const view::Terminal_window *win, const std::string &text )
    :Constraint(win, text)
{

}

int Constraint_center::constrain_x()
{
    int text_len = static_cast<int>(m_text.size());
    int cols = m_win_ptr->cols();
    int lines = m_win_ptr->lines();

    if ( cols < text_len )
        return 1;
    // TODO bounds checking

    if (m_win_ptr->boxed())
    {
        lines--;cols--;
    }
    return (cols - text_len)/2;
}

int Constraint_center::constrain_y()
{
    // None
    return 0;
}

}
