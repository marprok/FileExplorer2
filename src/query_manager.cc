#include "../inc/query_manager.h"
#include "../inc/ScrollableVector.hpp"
#include <string>

namespace view
{

query_manager::query_manager() {}

std::string query_manager::take_input(TerminalWindow &win,
                                      const std::string &prompt)
{
    win.box('#','#');
    keypad(*win, true);
    /* Handle the keyboard input */
    std::string text;
    int key = 0;
    bool end = false;
    while (!end)
    {
        win.erase();
        win.rebox();
        win.mvwprintw(1, 1, prompt);
        win.mvwprintw(2, 1, text);
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

bool query_manager::ask(TerminalWindow &win,
         const std::string &question)
{
    bool choice = false;
    win.box('#','#');
    keypad(*win, true);
    /* Handle the keyboard input */
    int key;
    bool choice_made = false;
    while (!choice_made)
    {
        win.erase();
        win.refresh();
        win.rebox();
        win.mvwprintw(1, 1, question);
        win.mvwprintw(2, 1, "[Y]ES  [N]O");
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
}
