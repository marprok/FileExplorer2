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

    scene.add_window(view::Orientation::LEFT, screen_lines, screen_cols/2, 0,0);
    scene.add_window(view::Orientation::RIGHT, screen_lines, screen_cols/2, 0, screen_cols/2);
    scene[view::Orientation::LEFT].box(0,0);
    scene[view::Orientation::RIGHT].box(0, 0);



    scene.refresh();

    while (true)
    {
        scene[view::Orientation::LEFT].mvwprintw(1,1, "Lovecraft");
        scene[view::Orientation::RIGHT].mvwprintw(1,1, "Cthulhu");
        scene.refresh();
        switch (wgetch(*scene[view::Orientation::LEFT]))
        {
        case KEY_UP:
            break;
        case KEY_DOWN:
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
