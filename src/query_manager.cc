#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "../inc/query_manager.h"
#include "../inc/scroll_vector.hpp"

namespace view
{

    QueryManager::QueryManager() {}

    std::string QueryManager::take_input(TerminalWindow &win,
                                          const std::string &prompt)
    {
        keypad(*win, true);
        /* Handle the keyboard input */
        std::string text;
        int key = 0;
        bool end = false;
        while (!end)
        {
            win.erase();
            win.print_center(0, prompt);
            win.print_center(1, text);
            win.refresh();
            key = wgetch(*win);
            switch (key)
            {
                case KEY_BACKSPACE:
                    if (text.size() > 0 )
                        text = text.substr(0, text.size() - 1);
                    break;
                case KEY_RESIZE:
                    win.resize();
                    break;
                case '\n':
                    end = true;
                    break;
                default:
                    if (static_cast<int>(text.size()) + 2 < win.cols())
                        text += static_cast<char>(key);
                    break;
            }
        }
        /* clear the screen */
        win.erase();
        win.refresh();

        return text;
    }

    bool QueryManager::ask(TerminalWindow &win,
                            const std::string &question)
    {
        bool choice = false;
        keypad(*win, true);
        /* Handle the keyboard input */
        int key;
        bool choice_made = false;
        while (!choice_made)
        {
            win.erase();
            win.print_center(0, question);
            win.print_center(1, "[Y]ES  [N]O");
            win.refresh();
            key = wgetch(*win);
            switch (key)
            {
                case 'y':
                case 'Y':
                    choice_made = true;
                    choice = true;
                    break;
                case 'n':
                case 'N':
                    choice_made = true;
                    choice = false;
                    break;
                case KEY_RESIZE:
                    win.resize();
                    break;
                default:
                    break;
            }
        }

        /* Bring back the state of the scene */
        win.erase();
        win.refresh();
        return choice;
    }

    std::size_t QueryManager::select(TerminalWindow &win, const std::vector<std::string> &choices)
    {

        auto calculate_lines = [&]() -> std::size_t
                               {
                                   if (choices.empty())
                                       return 0;
                                   std::size_t output_lines = 0;
                                   if (win.lines() > 2)
                                       output_lines = std::min(static_cast<std::size_t>(win.lines() - 2),
                                                               choices.size());
                                   return output_lines;
                               };
        std::size_t visible_lines = calculate_lines();
        utils::scrollable_vector<std::string> sv(0, visible_lines, choices);

        keypad(*win, true);
        /* Handle the keyboard input */
        int key;
        bool choice_made = false;
        while (!choice_made)
        {
            win.erase();
            for (std::size_t i = 0; i < visible_lines && i < sv.size(); ++i)
            {
                if (i == sv.index())
                    win.print_center(static_cast<int>(i), sv[i], A_REVERSE);
                else
                    win.print_center(static_cast<int>(i), sv[i]);
            }
            win.refresh();
            key = wgetch(*win);
            switch (key)
            {
                case '\n':
                    choice_made = true;
                    break;
                case KEY_UP:
                    sv.move_up();
                    break;
                case KEY_DOWN:
                    sv.move_down();
                    break;
                case KEY_RESIZE:
                    win.resize();
                    visible_lines = calculate_lines();
                    sv.reset(0, visible_lines, choices);
                    break;
                default:
                    break;
            }
        }

        win.erase();
        win.refresh();
        return sv.real_index(sv.index());
    }

    void QueryManager::wait(TerminalWindow &win, const std::string &msg, pid_t pid)
    {
        keypad(*win, true);
        /* Handle the keyboard input */
        int key;
        int dot = 0;
        bool end = false;
        while (!end)
        {
            int status = 0;
            pid_t p = waitpid(pid, &status, WNOHANG);
            if (p > 0)
                end = WIFEXITED(status);

            win.erase();
            if (dot == 0)
                win.print_center(0, msg + ".");
            else if (dot == 1)
                win.print_center(0, msg + "..");
            else if (dot == 2)
                win.print_center(0, msg + "...");

            dot = (dot+1)%3;
            win.refresh();
            key = wgetch(*win);
            switch (key)
            {
                case KEY_RESIZE:
                    win.resize();
                    break;
                default:
                    break;
            }
        }

        /* Bring back the state of the scene */
        win.erase();
        win.refresh();
    }
}
