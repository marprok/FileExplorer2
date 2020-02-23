#include "inc/scene.h"
#include "inc/scroll_vector.hpp"
#include "inc/File.h"
#include "inc/Directory.h"
#include <algorithm>
#include <string>
#include <stack>
#include <utility>


typedef std::pair<std::size_t, utils::scrollable_vector<fs::FS_Entry*>> scroll_pair;

enum POSITION
{
    LEFT,
    RIGHT,
    BOTTOM
};

static std::size_t calculate_lines(const view::Terminal_window& window, const std::vector<fs::FS_Entry*> &vec)
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

static void load_current(fs::Directory* current, std::vector<fs::FS_Entry*> &vec)
{
    if (!current)
        return;
    /* Prepare the data structures */
    current->load();
    vec.clear();
    if (current->empty())
        return;
    /* Load the vector */
    auto comp_file = [](auto& l, auto& r) { return l.name() < r.name();};
    auto comp_dir = [](auto& l, auto& r) { return l->name() < r->name();};
    std::sort(current->dirs().begin(), current->dirs().end(), comp_dir);
    std::sort(current->files().begin(), current->files().end(), comp_file);
    for (auto& dir : current->dirs())
        vec.push_back(dir);
    for (auto& file : current->files())
        vec.push_back(&file);
}

static void display_file_info(view::Terminal_window& window, const fs::File &file)
{
    window.print_left(1, "NAME: " + file.name());
    window.print_left(3, "SIZE: " + file.size() + "[Bytes]");
    window.print_left(5, "RIGHTS: " + file.rights());
    window.print_left(7, "LAST ACCESSED: " + file.last_acc());
    window.print_left(9, "LAST MODIFIED: " + file.last_mod());
    window.print_left(11, "INODE: " + file.inode_number());
    window.print_left(13, "HARD LINKS: " + file.hlinks_number());
    if (file.is_link())
        window.print_left(15, "[LINK]->" + file.real_file());
}

int main()
{
    std::size_t output_lines;
    /* Setup the scene */
    view::Scene &scene = view::Scene::the();
    scene.add_window(0.8f, 0.5f, 0.0f, 0.0f);
    scene.add_window(0.8f, 0.5f, 0.0f, 0.5f);
    scene.add_window(0.2f, 1.0f, 0.8f, 0.0f);
    scene[LEFT].box(0,0);
    scene[RIGHT].box(0, 0);
    scene[BOTTOM].box(0,0);
    /* Explicitly set the inout window */
    scene.set_input_window(LEFT);
    keypad(*scene.get_input_window(), true);
    /* Create the nececary data structures */
    std::vector<fs::FS_Entry*> vec;
    int key = 0;
    std::size_t index = 0;
    fs::Directory *root = new fs::Directory("/home", nullptr);
    fs::Directory *current = root;
    load_current(current, vec);
    /* -2 lines because the window is boxed */
    output_lines = std::min(static_cast<std::size_t>(scene[LEFT].lines() - 2),
                            vec.size());
    utils::scrollable_vector<fs::FS_Entry*> sv(0, output_lines, vec);
    std::stack<scroll_pair> scroll_stack;
    scroll_stack.push(scroll_pair(index, sv));

    auto is_directory = [](std::size_t i, const utils::scrollable_vector<fs::FS_Entry*> &sv, fs::Directory *current)
    {
        return sv.real_index(i) < current->dirs().size();
    };

    auto is_file = [](std::size_t i, const utils::scrollable_vector<fs::FS_Entry*> &sv, fs::Directory *current)
    {
        return sv.real_index(i) >= current->dirs().size();
    };

    while (key != KEY_END)
    {
        if (index >= output_lines)
            index = 0;
        /* Clear the windows and rebox them */
        scene.erase();
        scene.rebox();
        /* Draw things on the windows */
        if (!current->empty())
        {
            for (std::size_t i = 0; i < output_lines && i < sv.size(); ++i)
            {
                if (is_directory(i, sv, current))
                    wattron(*scene[LEFT], COLOR_PAIR(1));
                if (i == index)
                    wattron(*scene[LEFT], A_REVERSE);
                /* +1 because it is a boxed window */
                scene[LEFT].print_left(static_cast<int>(i+1), sv[i]->name());
                if (i == index)
                    wattroff(*scene[LEFT], A_REVERSE);
                if (is_directory(i, sv, current))
                    wattroff(*scene[LEFT], COLOR_PAIR(1));
            }
            if (is_file(index, sv, current))
            {
                /* Calculate the index of the file */
                std::size_t fi = sv.real_index(index) - current->dirs().size();
                display_file_info(scene[RIGHT], current->files()[fi]);
            }else if (!current->dirs().empty())
            {
                auto dir = current->dirs()[sv.real_index(index)];
                std::size_t dir_size = dir->load();
                if (!dir->empty())
                {
                    std::size_t right_lines = std::min(static_cast<std::size_t>(scene[RIGHT].lines() - 2),
                                                                                dir_size);
                    for (std::size_t i = 0; i < right_lines; ++i)
                    {
                        if (i < dir->dirs().size())
                        {
                            wattron(*scene[RIGHT], COLOR_PAIR(1));
                            scene[RIGHT].print_left(static_cast<int>(i+1),
                                                   dir->dirs()[i]->name());
                            wattroff(*scene[RIGHT], COLOR_PAIR(1));
                        }else if (!dir->files().empty())
                        {
                            std::size_t fi = i - dir->dirs().size();
                            scene[RIGHT].print_left(static_cast<int>(i+1),
                                                    dir->files()[fi].name());
                        }
                    }
                }else
                {
                    scene[RIGHT].print_center(scene[RIGHT].lines()/2, "EMPTY");
                }
            }
        }else
        {
            scene[LEFT].print_center(scene[LEFT].lines()/2, "EMPTY");
        }
        scene[BOTTOM].print_left(1, current->abs_path());
        scene[BOTTOM].print_left(2, "Files: " + std::to_string(current->files().size()) + " Directories: " + std::to_string(current->dirs().size()));
        /* Refresh the windowws and wait for an event */
        scene.refresh();
        if ((scene >> key) == ERR)
            break;
        switch (key)
        {
        case KEY_UP:
            if (!sv.needs_scroll_up(index))
                index--;
            else
                sv.scroll_up();
            break;
        case KEY_DOWN:
            if (!sv.needs_scroll_down(index))
                index++;
            else
                sv.scroll_down();
            break;
        case KEY_RIGHT:
            if (is_directory(index, sv, current))
            {
                scroll_stack.push(scroll_pair(index, sv));
                current = current->dive(sv.real_index(index));
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
                index = 0;
            }
            break;
        case KEY_LEFT:
        {
            if (!scroll_stack.empty())
            {
                auto sp = scroll_stack.top();
                sv = sp.second;
                index = sp.first;
                scroll_stack.pop();
            }
            current = current->surface();
            load_current(current, vec);
            output_lines = calculate_lines(scene[LEFT], vec);
            break;
        }
        case KEY_RESIZE:
            scene.resize();
            output_lines = calculate_lines(scene[LEFT], vec);
            sv.reset(0, output_lines, vec);
            break;
        case 'c':
        {
            std::string file_name = scene.take_input(0.2f, 0.30f, 0.45f, 0.35f, "Create file");
            if (file_name.size() != 0)
            {
                current->create_file(file_name);
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
            }
            break;
        }
        case 'd':
            if (!current->empty() && is_file(index, sv, current))
            {
                std::size_t file_i = sv.real_index(index) - current->dirs().size();
                std::string prompt = "Delete " + current->files()[file_i].name() + "?";
                bool choice = scene.ask(0.2f, 0.30f, 0.45f, 0.35f, prompt);
                if (choice)
                {
                    current->unlink_file(file_i);
                    load_current(current, vec);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    index = 0;
                    sv.reset(0, output_lines, vec);
                }
            }
            break;
        }
    }
    delete root;
    return 0;
}
