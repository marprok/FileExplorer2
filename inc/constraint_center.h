#ifndef CONSTRAINT_CENTER_H
#define CONSTRAINT_CENTER_H

#include "constraint.h"

namespace utils {
    class Constraint_center : public Constraint
    {
    public:
        Constraint_center( const view::Terminal_window *win, const std::string &text );

        int constrain_x();
        int constrain_y();
    };
}

#endif /* CONSTRAINT_CENTER_H */
