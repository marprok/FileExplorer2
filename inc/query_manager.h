#ifndef QUERY_MANAGER_H
#include <string>
#include <vector>
#include "terminal_window.h"

namespace view
{
    class Query_manager
    {
    public:
        Query_manager();

        std::string take_input(TerminalWindow &win, const std::string &prompt);

        bool ask(TerminalWindow &win, const std::string &question);

        std::size_t select(TerminalWindow &win, const std::vector<std::string> &choices);
    };
}
#endif
