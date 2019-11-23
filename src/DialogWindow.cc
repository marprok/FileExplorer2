#include "../inc/DialogWindow.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    DialogWindow::DialogWindow(int lines, int cols, float y, float x)
        :TerminalWindow(lines, cols, y, x)
    {}

    int DialogWindow::resize()
    {
        int ret;
        ret = this->erase();
        if (ret != OK)
            return ret;
        ret = this->refresh();
        if (ret != OK)
            return ret;
        ret = this->move();
        if (ret != OK)
            return ret;
        ret = this->rebox();
        if (ret != OK)
            return ret;
        return OK;
    }

    std::string DialogWindow::take_input(const std::string &prompt)
    {
        this->box('#','#');
        keypad(**this, true);
        /* Handle the keyboard input */
        std::string text;
        int key = 0;
        bool end = false;
        int scene_lines, scene_cols;
        while (!end)
        {
            getmaxyx(stdscr, scene_lines, scene_cols);
            if (scene_lines < m_lines || scene_cols < m_cols)
                break;
            this->erase();
            this->rebox();
            this->mvwprintw(1,
                            (m_cols - static_cast<int>(prompt.size()))/2,
                            prompt);
            this->mvwprintw(2,1, text);
            this->refresh();
            key = wgetch(**this);
            switch (key)
            {
            case KEY_BACKSPACE:
                if (text.size() > 0 )
                    text = text.substr(0, text.size() - 1);
                break;
            case KEY_RESIZE:
                this->resize();
                break;
            case '\n':
                end = true;
                break;
            default:
                if (static_cast<int>(text.size()) + 2 < m_cols)
                    text += static_cast<char>(key);
                break;
            }
        }
        /* clear the screen */
        this->erase();
        this->refresh();

        return text;
    }

    bool DialogWindow::ask(const std::string &question)
    {
        bool choice = false;
        this->box('#','#');
        keypad(**this, true);
        /* Handle the keyboard input */
        int key;
        bool choice_made = false;
        int scene_lines, scene_cols;
        while (!choice_made)
        {
            this->erase();
            this->refresh();
            this->rebox();
            this->mvwprintw(1,
                            (m_cols - static_cast<int>(question.size()))/2,
                            question);
            this->mvwprintw(2, m_cols/2 - 7, "[Y]ES  [N]O");
            key = wgetch(**this);
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
                getmaxyx(stdscr, scene_lines, scene_cols);
                if (scene_lines < m_lines || scene_cols < m_cols)
                    choice_made = true;
                else
                    this->resize();
                break;
            default:
                break;
            }
        }

        /* Bring back the state of the scene */
        this->erase();
        this->refresh();
        return choice;
    }

    std::size_t DialogWindow::choose(const std::vector<std::string> &choices)
    {
        (void)choices;
        return 0;
    }
}
