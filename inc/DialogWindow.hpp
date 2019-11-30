#ifndef DWINDOW_H
#define DWINDOW_H

#include <ncurses.h>
#include <string>
#include <vector>
#include "Scene.hpp"
#include "TerminalWindow.hpp"

namespace view
{
/**
 * @brief The DialogWindow class
 *  This class represents windows that
 *  are not to be permanent on the screen,
 *  but instead perform some kind of user interaction
 *  and then be destroyed.
 *
 */
class DialogWindow : public TerminalWindow
{
public:
    /**
     * @brief take_input
     * @param prompt
     * @return the text entered
     */
    std::string take_input(const std::string &prompt);
    /**
     * @brief ask
     * @param question
     * @return true if yes was selected
     */
    bool ask(const std::string &question);
    /**
     * @brief choose
     * @param choices
     * @param scroll_size
     * @param msg
     * @return the choice made
     */
    std::size_t choose(const std::vector<std::string> &choices, std::size_t scroll_size, std::string& msg);
    DialogWindow(const DialogWindow& other) = delete;
    DialogWindow& operator=(const DialogWindow& other) = delete;
    DialogWindow(DialogWindow&& other) = delete;
    DialogWindow& operator=(DialogWindow&& other) = delete;
    int resize() override;
    friend class Scene;
private:
    bool m_centered;
    /**
     * @brief DialogWindow
     * @param lines
     * @param cols
     * @param y
     * @param x
     * @param centered true if the window must be centered(x is ignored)
     */
    DialogWindow(int lines, int cols, float y, float x, bool centered = false);
};
}
#endif
