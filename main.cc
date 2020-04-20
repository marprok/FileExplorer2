#include <algorithm>
#include <string>
#include <chrono>
#include <cassert>
#include <unistd.h>
#include <pwd.h>
#include "inc/scene.h"
#include "inc/scroll_vector.hpp"
#include "inc/file.h"
#include "inc/directory.h"
#include "inc/node.h"

enum POSITION
{
    LEFT,
    RIGHT,
    BOTTOM
};

static std::size_t calculate_lines(const view::Terminal_window& window, const std::vector<fs::Node*> &vec)
{
    if (vec.empty())
        return 0;
    std::size_t output_lines = 0;
    if (window.lines() > 2)
        output_lines = std::min(static_cast<std::size_t>(window.lines() - 2), vec.size());
    return output_lines;
}

static void load_current(fs::Node* current, std::vector<fs::Node*> &vec)
{
    if (!current)
        return;

    current->load();
    vec.clear();
    if (current->empty())
        return;

    // TODO: maybe support sorting by size?
    std::sort(current->dirs().begin(), current->dirs().end(), [] (const auto& l, const auto& r) { return l->abs_path() < r->abs_path(); });
    std::sort(current->files().begin(), current->files().end(), [] (const auto& l, const auto& r) { return l->abs_path() < r->abs_path(); });

    for (auto& dir : current->dirs())
        vec.push_back(dir);

    for (auto& file : current->files())
        vec.push_back(file);

}

static void display_file_info(view::Terminal_window& window, fs::Node* node)
{
    assert(node);
    auto inode = node->inode();
    assert(inode);
    window.print_left(1, "NAME: ", COLOR_PAIR(3));
    window.print(inode->name(), A_UNDERLINE);

    window.print_left(3, "SIZE: ", COLOR_PAIR(3));
    window.print(inode->size() + "[Bytes]", A_UNDERLINE);

    window.print_left(5, "RIGHTS: ", COLOR_PAIR(3));
    window.print( inode->rights(), A_UNDERLINE);

    window.print_left(7, "LAST ACCESSED: ", COLOR_PAIR(3));
    window.print(inode->last_accessed(), A_UNDERLINE);

    window.print_left(9, "LAST MODIFIED: ", COLOR_PAIR(3));
    window.print(inode->last_modified(), A_UNDERLINE);

    window.print_left(11, "LAST CHANGED: ", COLOR_PAIR(3));
    window.print( inode->last_status_changed(), A_UNDERLINE);

    window.print_left(13, "INODE: ", COLOR_PAIR(3));
    window.print(inode->inode_number(), A_UNDERLINE);

    window.print_left(15, "HARD LINKS: ", COLOR_PAIR(3));
    window.print(inode->hard_link_count(), A_UNDERLINE);

    if (inode->is_symbolic_link())
    {
        window.print_left(17, "[LINK]-> ", COLOR_PAIR(3));
        window.print(inode->real_name(node->abs_path()), A_UNDERLINE);
    }
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
    std::vector<fs::Node*> vec;
    int key = 0;
    std::size_t index = 0;
    fs::Node *root = new fs::Node(new fs::Directory("/home/void"), nullptr);
    fs::Node *current = root;
    load_current(current, vec);
    output_lines = calculate_lines(scene[LEFT], vec);
    utils::scrollable_vector<fs::Node*> sv(0, output_lines, vec);
    std::vector<fs::Node*> selection;

    // take the name of the current user
    uid_t uid = getuid();
    std::string user = "UNKNOWN";
    struct passwd* pw = getpwuid(uid);
    if (pw)
    {
        user = pw->pw_name;
    }

    std::time_t time;
    bool do_update = true;
    char time_buf[32] = {0};

    while (key != KEY_END)
    {
        index = sv.index();
        fs::Node* selected_element = sv[index];
        if (index >= output_lines)
            index = 0;

        if (do_update)
        {
            /* Clear the windows and rebox them */
            scene.erase();
            scene.rebox();

            /* Draw things on the windows */
            if (!current->empty())
            {
                for (std::size_t i = 0; i < output_lines && i < sv.size(); ++i)
                {
                    // TODO: try to make this a little bit cleaner
                    attr_t attr = 0;
                    if (sv.is_selected(i))
                        attr = A_BLINK;

                    if (sv[i]->inode()->is_directory())
                        attr |= COLOR_PAIR(1);
                    else if (sv[i]->inode()->is_symbolic_link())
                        attr |= COLOR_PAIR(2);
                    else
                        attr |= COLOR_PAIR(4);

                    if (i == index)
                        attr |= A_REVERSE;

                    /* +1 because it is a boxed window */
                    scene[LEFT].print_left(static_cast<int>(i+1), sv[i]->inode()->name(), attr);
                    if (!sv[i]->inode()->is_directory())
                        scene[LEFT].print_right(static_cast<int>(i+1), sv[i]->inode()->formated_size(), COLOR_PAIR(2));
                }
                if (selected_element->inode()->is_regular_file() ||
                    selected_element->inode()->is_symbolic_link())
                {
                    display_file_info(scene[RIGHT], selected_element);
                }else if (selected_element->inode()->is_directory())
                {
                    std::size_t node_size = selected_element->load();
                    if (!selected_element->empty())
                    {
                        std::size_t right_lines = std::min(static_cast<std::size_t>(scene[RIGHT].lines() - 2), node_size);
                        std::size_t i = 0;

                        for (; i < selected_element->dirs().size() && i < right_lines; ++i)
                        {
                            scene[RIGHT].print_left(static_cast<int>(i+1), selected_element->dirs()[i]->inode()->name(), COLOR_PAIR(1));
                        }

                        for (std::size_t j = 0; j < selected_element->files().size() && i < right_lines; ++j, ++i)
                        {
                            auto inode = selected_element->files()[j]->inode();
                            if (inode->is_symbolic_link())
                                scene[RIGHT].print_left(static_cast<int>(i+1), inode->name(), COLOR_PAIR(2));
                            else
                                scene[RIGHT].print_left(static_cast<int>(i+1), inode->name(), COLOR_PAIR(4));

                            scene[RIGHT].print_right(static_cast<int>(i+1), selected_element->files()[j]->inode()->formated_size(), COLOR_PAIR(2));

                        }
                    }else
                        scene[RIGHT].print_center(scene[RIGHT].lines()/2, "EMPTY", A_REVERSE);
                }
            }else
                scene[LEFT].print_center(scene[LEFT].lines()/2, "EMPTY", A_REVERSE);
        }


        time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // we use this because std::ctime adds a newline in the end of the string...
        std::strftime(time_buf, sizeof(time_buf), "%c", std::localtime(&time));

        scene[BOTTOM].print_left(1, "[" + user + "] ", COLOR_PAIR(2));
        scene[BOTTOM].print(std::string(time_buf) + " ");
        scene[BOTTOM].print(std::to_string(current->files().size()), COLOR_PAIR(4));
        scene[BOTTOM].print("/", COLOR_PAIR(3));
        scene[BOTTOM].print(std::to_string(current->dirs().size()) + " ", COLOR_PAIR(1));
        scene[BOTTOM].print(current->inode()->rights(), COLOR_PAIR(5));
        scene[BOTTOM].print_left(2, current->abs_path(), A_UNDERLINE | COLOR_PAIR(3));


        /* Refresh the windowws and wait for an event */
        scene.refresh();
        if ((scene >> key) == ERR)
            do_update = false;
        else
        {
            do_update = true;
            switch (key)
            {
            case KEY_UP:
                sv.move_up();
                if (sv.selection_in_progress())
                    sv.selection_append();
                break;
            case KEY_DOWN:
                sv.move_down();
                if (sv.selection_in_progress())
                    sv.selection_append();
                break;
            case KEY_RIGHT:
                if (!current->empty() && selected_element->inode()->is_directory())
                {
                    if (sv.selection_in_progress())
                        sv.interupt_selection();
                    current = selected_element;
                    load_current(current, vec);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                }
                break;
            case KEY_LEFT:
            {
                if (!current->parent())
                    break;

               if (sv.selection_in_progress())
                   sv.interupt_selection();

                current = current->parent();
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
                break;
            }
            case KEY_RESIZE:
                scene.resize();
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
                break;
            case 'c':
            {
                std::vector<std::string> temp = { "File", "Directory" };
                auto ret = scene.select(0.2f, 0.30f, 0.45f, 0.35f, temp);
                auto name = scene.take_input(0.2f, 0.30f, 0.45f, 0.35f, "Create " + temp[ret]);
                auto confirm = scene.ask(0.2f, 0.30f, 0.45f, 0.35f, "Create " + name + "?");
                if (confirm)
                {
                    if (ret == 0)
                        current->create_file(name);
                    else
                        current->create_dir(name);
                    load_current(current, vec);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                }
                break;
            }
            case 'd':
                if (current->empty())
                    break;
                selected_element->remove();
                delete selected_element;
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
                break;
            case 'm':
                if (selection.empty())
                    break;

                for (auto& element : selection)
                    element->move(current);
                //TODO: current should update it's timestamps via stat
                load_current(current, vec);
                output_lines = calculate_lines(scene[LEFT], vec);
                sv.reset(0, output_lines, vec);
                break;
            case 's':
                sv.start_selection();
                break;
            case 'e':
                selection = sv.end_selection();
                break;
            }
        }
    }
    delete root;
    return 0;
}
