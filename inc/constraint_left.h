#ifndef CONSTRAINT_LEFT_H
#define CONSTRAINT_LEFT_H

#include "constraint.h"

namespace utils {
    class Constraint_left : public Constraint
    {
    public:
        Constraint_left( const view::Terminal_window *win, const std::string &text );

        int constrain_x();
        int constrain_y();
    };
}

#endif /* CONSTRAINT_LEFT_H */
