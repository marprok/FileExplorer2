#include <iostream>
#include <ncurses.h>
#include "inc/Scene.hpp"
#include "inc/scroll_vector.hpp"
#include "inc/File.hpp"
#include "inc/Directory.hpp"
#include <algorithm>

enum POSITION
{
    LEFT,
    RIGHT,
    BOTTOM
};

static std::size_t calculate_lines(view::TWindow& window, std::vector<std::string> &vec)
{
    if (vec.empty())
        return 0;
    std::size_t output_lines = 0;
    if (window.lines() > 2)
        output_lines = std::min(static_cast<std::size_t>(window.lines() - 2),
                                vec.size());
    else
        output_lines = 0;
    return output_lines;
}

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
    window.mvwprintw(1,1, "NAME: " + file.name());
    window.mvwprintw(3,1, "SIZE: " + file.size() + "[Bytes]");
    window.mvwprintw(5,1, "RIGHTS: " + file.rights());
    window.mvwprintw(7,1, "LAST ACCESSED: " + file.last_acc());
    window.mvwprintw(9,1, "LAST MODIFIED: " + file.last_mod());
    window.mvwprintw(11,1, "INODE: " + file.inode_number());
    window.mvwprintw(13,1, "HARD LINKS: " + file.hlinks_number());
    if (file.is_link())
        window.mvwprintw(15,1, "[LINK]->" + file.real_file());
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
    scene.add_window(0.22f, 1.0f, 0.8f, 0.0f, screen_lines, screen_cols);
    scene[LEFT].box(0,0);
    scene[RIGHT].box(0, 0);
    scene[BOTTOM].box(0,0);
    /* Explicitly set the inout window */
    scene.set_input_window(LEFT);
    keypad(*scene.get_input_window(), true);
    /* Create the nececary data structures */
    std::vector<std::string> vec;
    int key = 0;
    size_t index = 0;
    fs::Directory *root = new fs::Directory("/home/void", nullptr);
    fs::Directory *current = root;
    load_current(current, vec);
    /* -2 lines because the window is boxed */
    output_lines = std::min(static_cast<std::size_t>(scene[LEFT].lines() - 2),
                            vec.size());
    utils::SCRVector<std::string> scr(0, output_lines, vec);
    while (key != KEY_END)
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
                    wattron(*scene[LEFT], COLOR_PAIR(1));
                if (i == index)
                    wattron(*scene[LEFT], A_REVERSE);
                /* +1 because it is a boxed window */
                scene[LEFT].mvwprintw(static_cast<int>(i+1),
                                   1,
                                   scr[i]);
                if (i == index)
                    wattroff(*scene[LEFT], A_REVERSE);
                if (scr.real_index(i) < current->dirs().size())
                    wattroff(*scene[LEFT], COLOR_PAIR(1));
            }
            if (scr.real_index(index) >= current->dirs().size())
            {
                /* Calculate the index of the file */
                size_t fi = scr.real_index(index) - current->dirs().size();
                display_file_info(scene[RIGHT], current->files()[fi]);
            }
        }else
        {
            scene[LEFT].mvwprintw(scene[LEFT].lines()/2, scene[LEFT].cols()/2 - 2, "EMPTY");
        }
        scene[BOTTOM].mvwprintw(1,1, current->abs_path());
        /* Refresh the windowws and wait for an event */
        scene.refresh();
        if ((scene >> key) == ERR)
            break;
        switch (key)
        {
        case 'c':

            current->create_file(scene.crt_input_window(0.5f, 0.5f,
                                                        0.5f, 0.25f,
                                                        "Create File"));
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            scr.reset(0, output_lines, vec);
            break;
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
                output_lines = calculate_lines(scene[LEFT], vec);
                scr.reset(0, output_lines, vec);
            }
            break;
        case KEY_LEFT:
            current = current->surface();
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            scr.reset(0, output_lines, vec);
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, screen_lines, screen_cols);
            scene.resize(screen_lines, screen_cols);
            output_lines = calculate_lines(scene[LEFT], vec);
            scr.reset(0, output_lines, vec);
            break;
        }
    }
    delete root;
    endwin();
    return 0;
}
