#include <iostream>
#include <ncurses.h>
#include "inc/Scene.hpp"
#include "inc/scroll_vector.hpp"
#include "inc/File.hpp"
#include "inc/Directory.hpp"
#include <algorithm>

static void load_current(fs::Directory* current, std::vector<std::string> &vec)
{
    if (!current)
        return;
    /* Prepare the data structures */
    current->load_info();
    vec.clear();
    if (current->empty())
        return;
    /* Load the vector */
    auto comp_file = [](auto& l, auto& r) { return l.name() < r.name();};
    auto comp_dir = [](auto& l, auto& r) { return l->name() < r->name();};
    std::sort(current->dirs().begin(), current->dirs().end(), comp_dir);
    std::sort(current->files().begin(), current->files().end(), comp_file);
    for (auto& dirs : current->dirs())
        vec.push_back(dirs->name() + "/");
    for (auto& files : current->files())
        vec.push_back(files.name());
}

static void display_file_info(view::TWindow& window, fs::File &file)
{
    window.mvwprintw(1,1, file.name());
    window.mvwprintw(2,1, file.abs_path());
    window.mvwprintw(3,1, file.size());
    window.mvwprintw(4,1, file.rights());
    window.mvwprintw(5,1, file.last_acc());
    window.mvwprintw(6,1, file.last_mod());
    window.mvwprintw(7,1, file.inode_number());
    window.mvwprintw(8,1, file.hlinks_number());
    if (file.is_link())
    {
        window.mvwprintw(9,1, "LINK");
    }
}

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
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    /* Setup the scene */
    getmaxyx(stdscr, screen_lines, screen_cols);
    view::Scene scene(screen_lines, screen_cols);
    scene.add_window(0.8f, 0.5f, 0.0f, 0.0f, screen_lines, screen_cols);
    scene.add_window(0.8f, 0.5f, 0.0f, 0.5f, screen_lines, screen_cols);
    scene.add_window(0.2f, 1.0f, 0.8f, 0.0f, screen_lines, screen_cols);
    scene[0].box(0,0);
    scene[1].box(0, 0);
    scene[2].box(0,0);
    /* Explicitly set the inout window */
    scene.set_input_window(0);
    keypad(*scene.get_input_window(), true);
    /* Create the nececary data structures */
    std::vector<std::string> vec;
    int key;
    size_t index = 0;
    fs::Directory *root = new fs::Directory("/home/marios", nullptr);
    fs::Directory *current = root;
    load_current(current, vec);
    /* -2 lines because the window is boxed */
    output_lines = std::min(static_cast<std::size_t>(scene[0].lines() - 2),
                            vec.size());
    utils::SCRVector<std::string> scr(0, output_lines, vec);
    while (true)
    {
        /* In case the window is too small */
        if (output_lines > 0  && index >= output_lines)
            index = output_lines-1;
        else if (output_lines == 0)
            index = 0;
        /* Clear the windows and rebox them */
        scene.erase();
        scene.rebox();
        /* Draw things on the windows */
        if (!current->empty())
        {
            for (size_t i = 0; i < output_lines && i < scr.size(); ++i)
            {
                if (scr.real_index(i) < current->dirs().size())
                    wattron(*scene[0], COLOR_PAIR(1));
                if (i == index)
                    wattron(*scene[0], A_REVERSE);
                /* +1 because it is a boxed window */
                scene[0].mvwprintw(static_cast<int>(i+1),
                                   1,
                                   scr[i]);
                if (i == index)
                    wattroff(*scene[0], A_REVERSE);
                if (scr.real_index(i) < current->dirs().size())
                    wattroff(*scene[0], COLOR_PAIR(1));
            }
            if (scr.real_index(index) >= current->dirs().size())
            {
                /* Calculate the index of the file */
                size_t fi = scr.real_index(index) - current->dirs().size();
                display_file_info(scene[1], current->files()[fi]);
            }
        }else
        {
            scene[0].mvwprintw(scene[0].lines()/2, scene[0].cols()/2 - 2, "EMPTY");
        }
        scene[2].mvwprintw(1,1, current->abs_path());
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
            if (scr.real_index(index) < current->dirs().size())
            {
                current = current->dive(scr.real_index(index));
                load_current(current, vec);
                output_lines = std::min(static_cast<std::size_t>(scene[0].lines() - 2),
                                        vec.size());
                scr.reset(0, output_lines, vec);
            }
            break;
        case KEY_LEFT:
            current = current->surface();
            load_current(current, vec);
            output_lines = std::min(static_cast<std::size_t>(scene[0].lines() - 2),
                                    vec.size());
            scr.reset(0, output_lines, vec);
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, screen_lines, screen_cols);
            scene.resize(screen_lines, screen_cols);
            output_lines = std::min(static_cast<std::size_t>(scene[0].lines() - 2),
                                    vec.size());
            scr.reset(0, output_lines, vec);
            break;
        }
    }
    delete current;
    endwin();
    return 0;
}
