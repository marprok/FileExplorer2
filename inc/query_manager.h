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

        std::string take_input(Terminal_window &win, const std::string &prompt);

        bool ask(Terminal_window &win, const std::string &question);

        std::size_t select(Terminal_window &win, const std::vector<std::string> &choices);
    };
}
#endif
