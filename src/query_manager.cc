#include "../inc/query_manager.h"
#include "../inc/scroll_vector.hpp"
#include "../inc/constraint_center.h"
#include <string>

namespace view
{

query_manager::query_manager() {}

std::string query_manager::take_input(Terminal_window &win,
                                      const std::string &prompt)
{
    win.box('#','#');
    keypad(*win, true);
    /* Handle the keyboard input */
    std::string text;
    utils::Constraint_center constraint(&win, prompt);
    int key = 0;
    bool end = false;
    while (!end)
    {
        win.erase();
        win.rebox();
        win.mvwprintw(1, constraint.constrain_x(), prompt);
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

bool query_manager::ask(Terminal_window &win,
         const std::string &question)
{
    bool choice = false;
    win.box('#','#');
    keypad(*win, true);
    utils::Constraint_center constraint_quest(&win, question);
    std::string yes_no = "[Y]ES  [N]O";
    utils::Constraint_center constraint_y_n(&win, yes_no);
    /* Handle the keyboard input */
    int key;
    bool choice_made = false;
    while (!choice_made)
    {
        win.erase();
        win.refresh();
        win.rebox();
        win.mvwprintw(1, constraint_quest.constrain_x(), question);
        win.mvwprintw(2, constraint_y_n.constrain_x(), yes_no);
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
