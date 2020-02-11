#ifndef CONSTRAINT_RIGHT_H
#define CONSTRAINT_RIGHT_H

#include "constraint.h"

namespace utils {
    class Constraint_right : public Constraint
    {
    public:
        Constraint_right( const view::Terminal_window *win, const std::string &text );

        int constrain_x();
        int constrain_y();
    };
}

#endif /* CONSTRAINT_RIGHT_H */
