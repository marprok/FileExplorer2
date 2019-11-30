#include "../inc/DialogWindow.hpp"
#include "../inc/ScrollableVector.hpp"
#include <iostream>
#include <sstream>

namespace view
{
    DialogWindow::DialogWindow(int lines, int cols, float y, float x, bool centered)
        :TerminalWindow(lines, cols, y, x), m_centered(centered)
    {
        if (m_centered)
        {
            int scene_lines, scene_cols;
            getmaxyx(stdscr, scene_lines, scene_cols);
            float width_ratio = static_cast<float>(m_cols)/scene_cols;
            m_begin_x = 0.5f - width_ratio/2;
            this->move();

        }
    }

    int DialogWindow::resize()
    {
        int ret, scene_lines, scene_cols;
        getmaxyx(stdscr, scene_lines, scene_cols);
        ret = this->erase();
        if (ret != OK)
            return ret;
        ret = this->refresh();
        if (ret != OK)
            return ret;
        if (m_centered)
        {
            float width_ratio = static_cast<float>(m_cols)/scene_cols;
            m_begin_x = 0.5f - width_ratio/2;
        }
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
            this->erase();
            this->rebox();
            this->mvwprintw(1, 1, prompt);
            this->mvwprintw(2, 1, text);
            this->refresh();
            key = wgetch(**this);
            switch (key)
            {
            case KEY_BACKSPACE:
                if (text.size() > 0 )
                    text = text.substr(0, text.size() - 1);
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, scene_lines, scene_cols);
                if (scene_lines < m_lines || scene_cols < m_cols)
                    end = true;
                else
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
            this->mvwprintw(1, 1, question);
            this->mvwprintw(2, 1, "[Y]ES  [N]O");
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

    std::size_t DialogWindow::choose(const std::vector<std::string> &choices,
                                     std::size_t scroll_size,
                                     std::string& msg)
    {
        if (choices.size() == 0 ||
            scroll_size == 0)
            return 0;

        utils::ScrollableVector<std::string> sv(0, scroll_size,choices);
        std::size_t choice = 0;

        this->box('#','#');
        keypad(**this, true);
        /* Handle the keyboard input */
        int key;
        bool choice_made = false;
        int scene_lines, scene_cols;
        std::size_t index = 0;
        std::size_t output_lines = std::min(scroll_size, choices.size());

        while (!choice_made)
        {
            this->erase();
            this->refresh();
            this->rebox();
            this->mvwprintw(1 , 1, msg);
            for (std::size_t i = 0; i < output_lines; ++i)
            {
                if (index == i)
                     wattron(**this, A_REVERSE);
                this->mvwprintw(2 + i, 1, sv[i]);
                if (index == i)
                     wattroff(**this, A_REVERSE);
            }
            key = wgetch(**this);
            switch (key)
            {
            case '\n':
                choice = sv.real_index(index);
                choice_made = true;
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, scene_lines, scene_cols);
                if (scene_lines < m_lines || scene_cols < m_cols)
                    choice_made = true;
                else
                    this->resize();
                break;
            case KEY_UP:
                if (index > 0)
                    index--;
                else
                    sv.scroll_up();
                break;
            case KEY_DOWN:
                if (index < output_lines - 1)
                    index++;
                else
                    sv.scroll_down();
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
}
