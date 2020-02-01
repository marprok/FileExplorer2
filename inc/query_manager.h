#ifndef QUERY_MANAGER_H

#include "TerminalWindow.h"
#include <string>
#include <vector>

namespace view
{
    class query_manager
    {
    public:
        query_manager();

        std::string take_input(TerminalWindow &win,
                               const std::string &prompt);

        bool ask(TerminalWindow &win,
                 const std::string &question);
    };
}
#endif
