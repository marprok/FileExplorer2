#ifndef QUERY_MANAGER_H
#include <string>
#include <vector>
#include "terminal_window.h"
#include "unistd.h"

namespace view
{
    class QueryManager
    {
    public:
        QueryManager();

        std::string take_input(TerminalWindow &win, const std::string &prompt);

        bool ask(TerminalWindow &win, const std::string &question);

        std::size_t select(TerminalWindow &win, const std::vector<std::string> &choices);

        void wait(TerminalWindow &win, const std::string &msg, pid_t);
    };
}
#endif
