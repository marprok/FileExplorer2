#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
/**
 *  @brief The terminal window class.
 *  Every ncurses window that gets displayed in the screen.
 *  The position and size of the window are relative
 *  to the terminal window(0-1.0f).
 */
class TerminalWindow
{
protected:
    WINDOW* m_window;
    float   m_per_cols, m_per_lines;
    float   m_begin_x, m_begin_y;
    chtype  m_hor_ch;
    chtype  m_ver_ch;
    bool    m_boxed;
    int     m_cursor_x, m_cursor_y;

    /**
     * @brief _resize This is the actual resize method
     * @return returns the value of the internal function
     */
    int _resize();
public:
    /**
     * @brief operator *
     * @return returns the ncurses window pointer(ncurses C API compatibility)
     */
    WINDOW* operator*();
    /**
     * @brief refresh the window
     * @return returns the value of the internal function
     */
    int refresh();
    /**
     * @brief box the window
     * @param verch vertical char
     * @param horch horizontal char
     * @return returns the value of the internal function
     */
    int box(chtype verch, chtype horch);
    /**
     * @brief mvwprintw cursor move and print
     * @param y column position
     * @param x row position
     * @param text to print
     * @return returns the value of the internal function
     */
    int mvwprintw(int y, int x, const std::string& text);
    /**
     * @brief mvwprintw cursor move and print
     * @param y column position
     * @param x row position
     * @param text to print
     * @return returns the value of the internal function
     */
    int mvwprintw(int y, int x, const std::string&& text);
    /**
     * @brief print text
     * @param text text to be printed
     * @return returns the value of the internal function
     */
    int print(const std::string& text);
    /**
     * @brief print text
     * @param text text to be printed
     * @return returns the value of the internal function
     */
    int print(const std::string&& text);
    /**
     * @brief move the window
     * @param y relative y coordinate
     * @param x relative x coordinate
     * @return returns the value of the internal function
     */
    int move(float y, float x);
    /**
     * @brief move the window to it's starting position
     * @return returns the value of the internal function
     */
    int move();
    /**
     * @brief erase the contents of the window
     * @return returns the value of the internal function
     */
    int erase();
    /**
     * @brief rebox the window
     * @return returns the value of the internal function
     */
    int rebox();
    /**
     * @brief move_cursor moves the cursor
     * @param y absolute column coordinate
     * @param x absolute row coordinates
     * @return returns the value of the internal function
     */
    int move_cursor(int y, int x);
    /**
     * @return the columns of the window
     */
    int cols();
    /**
     * @return the lines of the window
     */
    int lines();
    /**
     * @brief delwin deletes the window pointer
     * @return returns the value of the internal function
     */
    int delwin();
    /**
     * @brief resize callback for the resize event
     * @return OK in case of success
     */
     int resize();
     /**
     * TODO: ensure that the WINDOW pointers are moved and
     * not copied.
     */
     /* Default copy constructors/assignment operators */
     TerminalWindow(const TerminalWindow& other) = default;
     TerminalWindow& operator=(const TerminalWindow& other) = default;
     TerminalWindow(TerminalWindow&& other) = default;
     TerminalWindow& operator=(TerminalWindow&& other) = default;
protected:
    TerminalWindow(float lines, float cols, float begin_y,
                   float begin_x);
    TerminalWindow();
    /**
     * @brief reset the window state to a new one
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     */
    void reset(float lines, float cols, float begin_y,
               float begin_x);

    friend class Scene;
};
}
#endif
