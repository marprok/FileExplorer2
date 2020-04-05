#include <string>
#include <cassert>
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
        output_lines = std::min(static_cast<std::size_t>(window.lines() - 2),
                                vec.size());
    return output_lines;
}

static void load_current(fs::Node* current, std::vector<fs::Node*> &vec)
{
    if (!current)
        return;
    /* Prepare the data structures */
    current->load();
    vec.clear();
    if (current->empty())
        return;
    /* Load the vector */
    auto head = current->dirs().head();
    while (head)
    {
        vec.push_back(head->data());
        head = head->next();
    }

    head = current->files().head();
    while (head)
    {
        vec.push_back(head->data());
        head = head->next();
    }

}

static void display_file_info(view::Terminal_window& window, fs::Node* node)
{
    assert(node);
    auto inode = node->inode();
    assert(inode);
    window.print_left(1, "NAME: " + inode->name());
    window.print_left(3, "SIZE: " + inode->size() + "[Bytes]");
    window.print_left(5, "RIGHTS: " + inode->rights());
    window.print_left(7, "LAST ACCESSED: " + inode->last_accessed());
    window.print_left(9, "LAST MODIFIED: " + inode->last_modified());
    window.print_left(11, "LAST CHANGED: " + inode->last_status_changed());
    window.print_left(13, "INODE: " + inode->inode_number());
    window.print_left(15, "HARD LINKS: " + inode->hard_link_count());
    if (inode->is_symbolic_link())
        window.print_left(17, "[LINK]->" + inode->real_name(node->abs_path()));
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
    /* -2 lines because the window is boxed */
    output_lines = std::min(static_cast<std::size_t>(scene[LEFT].lines() - 2),
                            vec.size());
    utils::scrollable_vector<fs::Node*> sv(0, output_lines, vec);
    std::vector<fs::Node*> selection;

    while (key != KEY_END)
    {
        index = sv.index();
        fs::Node* selected_element = sv[index];
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
                // TODO: try to make this a little bit cleaner
                if (sv.is_selected(i))
                    wattron(*scene[LEFT], COLOR_PAIR(2));
                else if (sv[i]->inode()->is_directory())
                    wattron(*scene[LEFT], COLOR_PAIR(1));
                if (i == index)
                    wattron(*scene[LEFT], A_REVERSE);
                /* +1 because it is a boxed window */
                scene[LEFT].print_left(static_cast<int>(i+1), sv[i]->inode()->name());
                if (i == index)
                    wattroff(*scene[LEFT], A_REVERSE);

                if (sv.is_selected(i))
                    wattroff(*scene[LEFT], COLOR_PAIR(2));
                else if (sv[i]->inode()->is_directory())
                    wattroff(*scene[LEFT], COLOR_PAIR(1));
            }
            if (selected_element->inode()->is_regular_file() ||
                selected_element->inode()->is_symbolic_link() )
            {
                /* Calculate the index of the file */
                auto tmp = current->files().get(selected_element);
                display_file_info(scene[RIGHT], tmp->data());
            }else if (selected_element->inode()->is_directory())
            {
                std::size_t node_size = selected_element->load();
                if (!selected_element->empty())
                {
                    std::size_t right_lines = std::min(static_cast<std::size_t>(scene[RIGHT].lines() - 2), node_size);
                    std::size_t i = 0;
                    auto head = selected_element->dirs().head();
                    while ( head && i < right_lines)
                    {
                        wattron(*scene[RIGHT], COLOR_PAIR(1));
                        scene[RIGHT].print_left(static_cast<int>(i+1), head->data()->inode()->name());
                        wattroff(*scene[RIGHT], COLOR_PAIR(1));
                        i++;
                        head = head->next();
                    }

                    head = selected_element->files().head();
                    while ( head && i < right_lines)
                    {
                        scene[RIGHT].print_left(static_cast<int>(i+1), head->data()->inode()->name());
                        i++;
                        head = head->next();
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
            if (selected_element->inode()->is_directory())
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
            std::vector<std::string> temp;
            for (int i = 0; i < 10; ++i)
                temp.push_back(std::to_string(i));
            std::size_t choice = scene.select(0.2f, 0.30f, 0.45f, 0.35f, temp);
//            std::string file_name = scene.take_input(0.2f, 0.30f, 0.45f, 0.35f, "Create file");
//            if (file_name.size() != 0)
//            {
//                current->create_file(file_name);
//                load_current(current, vec);
//                output_lines = calculate_lines(scene[LEFT], vec);
//                sv.reset(0, output_lines, vec);
//            }
            break;
        }
        case 'd':
            if (current->empty())
                break;
            current->remove(selected_element);
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
    delete root;
    return 0;
}
