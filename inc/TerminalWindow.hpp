#ifndef TWINDOW_H
#define TWINDOW_H

#include <ncurses.h>
#include <string>

namespace view
{
/**
 *  @brief The terminal window superclass.
 *  Every ncurses window that gets displayed in the screen.
 *  The position of the window is given relative to the window(0-1.0f)
 *  on both axis. The size of the window is fixed and it is up to the
 *  subclass to implement a resize method that should be called when
 *  the resize event is received.
 */
class TerminalWindow
{
protected:
    WINDOW* m_window;
    int     m_lines,m_cols;
    float   m_begin_x, m_begin_y;
    chtype  m_hor_ch;
    chtype  m_ver_ch;
    bool    m_boxed;
    int     m_cursor_x, m_cursor_y;
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
    virtual int resize() = 0;
protected:
    /* Default copy constructors/assignment operators */
    TerminalWindow(const TerminalWindow& other) = default;
    TerminalWindow& operator=(const TerminalWindow& other) = default;
    TerminalWindow(TerminalWindow&& other) = default;
    TerminalWindow& operator=(TerminalWindow&& other) = default;

    TerminalWindow(int lines, int cols, float begin_y,
                   float begin_x);
    TerminalWindow();
    /**
     * @brief reset the window state to a new one
     * @param lines
     * @param cols
     * @param begin_y
     * @param begin_x
     */
    void reset(int lines, int cols, float begin_y,
               float begin_x);
};
}
#endif
