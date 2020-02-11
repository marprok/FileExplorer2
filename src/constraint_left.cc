#include "../inc/constraint_left.h"

namespace utils {

Constraint_left::Constraint_left( const view::Terminal_window *win, const std::string &text )
    :Constraint(win, text)
{

}

int Constraint_left::constrain_x()
{
    if (m_win_ptr->boxed())
        return 1;
    return 0;
}

int Constraint_left::constrain_y()
{
    // None
    return 0;
}

}
