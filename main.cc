#include <iostream>
#include <ncurses.h>
#include "inc/Scene.hpp"
#include "inc/ScrollableVector.hpp"
#include "inc/File.hpp"
#include "inc/Directory.hpp"
#include <algorithm>
#include <string>

enum POSITION
{
    LEFT,
    RIGHT,
    BOTTOM
};

static std::size_t calculate_lines(view::ResizableWindow& window, std::vector<std::string> &vec)
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

static void display_file_info(view::ResizableWindow& window, fs::File &file)
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
    std::size_t output_lines;
    /* Global curses initialization */
    initscr();
    cbreak();
    noecho();
    curs_set(0); /* Make the cursor invisible. */
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    /* Setup the scene */
    view::Scene scene;
    scene.add_window(0.8f, 0.5f, 0.0f, 0.0f);
    scene.add_window(0.8f, 0.5f, 0.0f, 0.5f);
    scene.add_window(0.22f, 1.0f, 0.8f, 0.0f);
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
    fs::Directory *root = new fs::Directory("/home", nullptr);
    fs::Directory *current = root;
    load_current(current, vec);
    /* -2 lines because the window is boxed */
    output_lines = std::min(static_cast<std::size_t>(scene[LEFT].lines() - 2),
                            vec.size());
    utils::ScrollableVector<std::string> sv(0, output_lines, vec);
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
            for (size_t i = 0; i < output_lines && i < sv.size(); ++i)
            {
                if (sv.real_index(i) < current->dirs().size())
                    wattron(*scene[LEFT], COLOR_PAIR(1));
                if (i == index)
                    wattron(*scene[LEFT], A_REVERSE);
                /* +1 because it is a boxed window */
                scene[LEFT].mvwprintw(static_cast<int>(i+1),
                                      1,
                                      sv[i]);
                if (i == index)
                    wattroff(*scene[LEFT], A_REVERSE);
                if (sv.real_index(i) < current->dirs().size())
                    wattroff(*scene[LEFT], COLOR_PAIR(1));
            }
            if (sv.real_index(index) >= current->dirs().size())
            {
                /* Calculate the index of the file */
                size_t fi = sv.real_index(index) - current->dirs().size();
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

            current->create_file(scene.take_input(4,
                                                  20,
                                                  0.5f,
                                                  0.45f,
                                                  "Create File"));
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            sv.reset(0, output_lines, vec);
            break;
        case 'd':
            if (!current->empty() &&
                    sv.real_index(index) >= current->dirs().size())
            {
                std::size_t file_i = sv.real_index(index) - current->dirs().size();
                std::string prompt = "Delete " + current->files()[file_i].name() + "?";
                bool choice = scene.ask(4,
                                        static_cast<int>(prompt.size()) + 2,
                                        0.5f,
                                        0.45f,
                                        prompt);
                if (choice)
                {
                    current->unlink_file(file_i);
                    load_current(current, vec);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                }
            }
            break;
        case 'h':
            /* demo of the choose functionality */
            scene.choose(vec, 10, "Choose one");
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            sv.reset(0, output_lines, vec);
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
        case KEY_RIGHT:
            if (sv.real_index(index) < current->dirs().size())
            {
                current = current->dive(sv.real_index(index));
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
            }
            break;
        case KEY_LEFT:
            current = current->surface();
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            sv.reset(0, output_lines, vec);
            break;
        case KEY_RESIZE:
            scene.resize();
            output_lines = calculate_lines(scene[LEFT], vec);
            sv.reset(0, output_lines, vec);
            break;
        }
    }
    delete root;
    endwin();
    return 0;
}
