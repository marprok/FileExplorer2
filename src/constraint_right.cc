#include "../inc/constraint_right.h"

namespace utils {

Constraint_right::Constraint_right( const view::Terminal_window *win, const std::string &text )
    :Constraint(win, text)
{

}

int Constraint_right::constrain_x()
{
    int text_len = static_cast<int>(m_text.size());
    int cols = m_win_ptr->cols();
    int lines = m_win_ptr->lines();

    if (cols < text_len)
        return 1;

    if (m_win_ptr->boxed())
    {
        lines--;cols--;
    }
    return cols - text_len;
}

int Constraint_right::constrain_y()
{
    // None
    return 0;
}

}
