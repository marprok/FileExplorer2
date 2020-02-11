#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "terminal_window.h"
#include <string>

namespace utils {

class Constraint
{
protected:
    const view::Terminal_window *m_win_ptr;
    std::string m_text;
public:
    Constraint( const view::Terminal_window *win, const std::string &text )
        :m_win_ptr(win), m_text(text)
    {

    }

    virtual ~Constraint()
    {

    }

    virtual int constrain_x() = 0;
    virtual int constrain_y() = 0;
};

}
#endif /* CONSTRAINT_H */
