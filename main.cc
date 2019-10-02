#include <iostream>
#include <ncurses.h>
#include "inc/Scene.hpp"
#include "inc/scroll_vector.hpp"
#include "inc/File.hpp"

int main()
{
    int screen_lines, screen_cols;
    std::size_t output_lines;
    /* Global curses initialization */
    initscr();
    cbreak();
    noecho();
    curs_set(0); /* Make the cursor invisible. */
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

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

    std::vector<int> vec;
    vec.clear();
    for (int i = 0; i < 2*screen_lines; ++i)
    {
        vec.emplace_back(i);
    }
    /* -2 lines because the window is boxed */
    output_lines = static_cast<std::size_t>(screen_lines - 2);

    utils::SCRVector<int> scr(0, output_lines, vec);
    int key;
    size_t index = 0;

    fs::File f("main.cc", "/home/void/Src/FileExplorer2");

    while (true)
    {
        /* Clear the windows and rebox them */
        scene.erase();
        scene.rebox();
        /* Draw things on the windows */
        if (index >= output_lines)
            index = output_lines-1;
        for (size_t i = 0; i < output_lines; ++i)
        {
            if (i == index)
                wattron(*scene[0], A_REVERSE);
            scene[0].mvwprintw(static_cast<int>(i+1),
                               1,
                               std::to_string(scr[i]));
            if (i == index)
                wattroff(*scene[0], A_REVERSE);
        }
        scene[1].mvwprintw(1,1, f.name());
        scene[1].mvwprintw(2,1, f.abs_name());
        scene[1].mvwprintw(3,1, f.size());
        scene[1].mvwprintw(4,1, f.rights());
        scene[1].mvwprintw(5,1, f.last_acc());
        scene[1].mvwprintw(6,1, f.last_mod());
        scene[1].mvwprintw(7,1, f.inode_number());
        scene[1].mvwprintw(8,1, f.hlinks_number());


        /* Refresh the windowws and wait for an event */
        scene.refresh();
        if ((scene >> key) == ERR)
            break;
        switch (key)
        {
        case KEY_UP:
            if (index > 0)
                index--;
            else
                scr.scroll_up();
            break;
        case KEY_DOWN:
            if (index < output_lines - 1)
                index++;
            else
                scr.scroll_down();
            break;
        case KEY_RIGHT:
            scene.crt_input_window(0.5, 0.5, 0.25, 0.25, "el fef");
            getmaxyx(stdscr, screen_lines, screen_cols);
            vec.clear();
            output_lines = static_cast<std::size_t>(screen_lines - 2);
            for (int i = 0; i < 2*screen_lines; ++i)
            {
                vec.emplace_back(i);
            }
            scr = utils::SCRVector<int>(0, output_lines, vec);
            break;
        case KEY_LEFT:
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, screen_lines, screen_cols);
            scene.resize(screen_lines, screen_cols);
            vec.clear();
            for (int i = 0; i < 2*screen_lines; ++i)
            {
                vec.emplace_back(i);
            }
            output_lines = static_cast<std::size_t>(screen_lines - 2);
            scr = utils::SCRVector<int>(0, output_lines, vec);
            break;
        }
    }

    endwin();
    return 0;
}
