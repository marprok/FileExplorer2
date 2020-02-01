#ifndef QUERY_MANAGER_H

#include "terminal_window.h"
#include <string>
#include <vector>

namespace view
{
    class query_manager
    {
    public:
        query_manager();

        std::string take_input(Terminal_window &win,
                               const std::string &prompt);

        bool ask(Terminal_window &win,
                 const std::string &question);
    };
}
#endif
