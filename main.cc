#include <iostream>
#include <ncurses.h>
#include "inc/Scene.hpp"

int main()
{
    int screen_lines, screen_cols;
    /* Global curses initialization */
    initscr();
    cbreak();
    noecho();
    curs_set(0); /* Make the cursor invisible. */
    /* Setup the scene */
    getmaxyx(stdscr, screen_lines, screen_cols);
    view::Scene scene(screen_lines, screen_cols);
    scene.add_window(1.0f, 0.5f, 0.0f, 0.0f, screen_lines, screen_cols);
    scene.add_window(1.0f, 0.5f, 0.0f, 0.5f, screen_lines, screen_cols);
    scene[0].box(0,0);
    scene[1].box(0, 0);
    /* Explicitly set the inout window. */
    scene.set_input_window(0);
    keypad(*scene.get_input_window(), true);
    scene.refresh();

    int key;
    std::string text("Lovecraft");
    while (true)
    {
        scene[1].mvwprintw(1,1, "Cthulhu");
        scene[0].mvwprintw(1,1, text);
        scene.refresh();
        if ((scene >> key) == ERR)
            break;
        switch (key)
        {
        case KEY_UP:
            break;
        case KEY_DOWN:
            text = scene.crt_input_window(0.5f, 0.5f, 0.25f, 0.25f, "TITLE");
            break;
        case KEY_RIGHT:
            break;
        case KEY_LEFT:
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, screen_lines, screen_cols);
            scene.resize(screen_lines, screen_cols);
            break;
        }
    }

    endwin();
    return 0;
}
