#ifndef SCENE_H
#define SCENE_H

#include <ncurses.h>
#include <vector>
#include "ResizableWindow.hpp"

namespace view
{
/**
 * @brief The Scene class
 *  This class represents the terminal screen.
 *  It is the only class that is allowed to create
 *  and destroy windows. Every fucntionality of the
 *  windows can be accessed by this class such as
 *  asking the user for input, resizing, refreshing etc.
 *  All the windows that are permanent to the scene
 *  are ResizableWindows.
 */
class Scene
{
private:
    std::vector<ResizableWindow> m_windows;
    size_t               m_input_window;
public:
    Scene();
    /**
     * @brief operator []
     * @param i
     * @return the window coresponding to the index i
     */
    ResizableWindow& operator[](size_t i);
    /**
     * @brief Deallocate all the windows
     */
    ~Scene();
    /**
     * @brief refresh all the windows
     * @return OK in case of success
     */
    int refresh();
    /**
     * @brief add_window Creates a new window
     * @param perlines
     * @param percols
     * @param begin_y
     * @param begin_x
     */
    void add_window(float perlines, float percols, float begin_y,
                    float begin_x);
    /**
     * @brief resize Resizes all the windows
     * @return OK in case of success
     */
    int resize();
    /**
     * @brief erase Erases the contents of all the windows
     * @return OK in case of success
     */
    int erase();
    /**
     * @brief rebox Rebox all windows
     * @return OK in case of success
     */
    int rebox();
    /**
     * @brief set_input_window Sets the input window
     * @param i The index of the chosen window
     * @return OK in case of success
     */
    int set_input_window(size_t i);
    /**
     * @brief get_input_window returns the input window
     * @return The input window
     */
    ResizableWindow& get_input_window();
    /**
     * @brief operator >>
     * @param key ouputs the key pressed
     * @return OK in case of success
     */
    int operator>>(int &key);
    /**
     * @brief take_input Takes input from the user
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     * @param prompt
     * @return the texted entered
     */
    std::string take_input(int lines, int cols, float begin_y,
                           float begin_x, const std::string& prompt);
    /**
     * @brief take_input Takes input from the user
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     * @param prompt
     * @return the text entered
     */
    std::string take_input(int lines, int cols, float begin_y,
                           float begin_x, std::string&& prompt);
    /**
     * @brief ask Asks the user a yes/now question
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     * @param prompt
     * @return true if yes was selected
     */
    bool ask(int lines, int cols, float begin_y,
             float begin_x,  std::string& prompt);
    /**
     * @brief ask Asks the user a yes/now question
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     * @param prompt
     * @return true if yes was selected
     */
    bool ask(int lines, int cols, float begin_y,
             float begin_x,  std::string&& prompt);
    /**
     * @brief choose an element from a vector
     * @param choices
     * @param scroll_size
     * @param prompt
     * @return the chosen element index
     */
    std::size_t choose(const std::vector<std::string> &choices,
                       std::size_t scroll_size,
                       std::string& prompt);
    /**
     * @brief choose an element from a vector
     * @param choices
     * @param scroll_size
     * @param prompt
     * @return the chosen element index
     */
    std::size_t choose(const std::vector<std::string> &choices,
                       std::size_t scroll_size,
                       std::string&& prompt);
};
}
#endif
