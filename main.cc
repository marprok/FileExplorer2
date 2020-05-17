#include <algorithm>
#include <string>
#include <chrono>
#include <cassert>
#include <unistd.h>
#include <pwd.h>
#include <linux/limits.h>
#include "inc/scene.h"
#include "inc/scroll_vector.hpp"
#include "inc/inode.h"
#include "inc/log.h"

enum POSITION
{
    LEFT,
    RIGHT,
    BOTTOM
};

static std::size_t calculate_lines(const view::Terminal_window& window, const std::vector<fs::Inode> &vec)
{
    if (vec.empty())
        return 0;
    std::size_t output_lines = 0;
    if (window.lines() > 2)
        output_lines = std::min(static_cast<std::size_t>(window.lines() - 2), vec.size());
    return output_lines;
}

static void load_current(fs::Inode &current,
                         std::vector<fs::Inode> &vec,
                         std::size_t *file_count = nullptr,
                         std::size_t *dir_count = nullptr)
{
    vec.clear();
    std::vector<fs::Inode> files, dirs;
    if (current.load(files, dirs) == 0)
    {
        utils::Log::the() << "load_current: empty current directory!\n";
        return;
    }
    if (file_count)
        *file_count = files.size();
    if (dir_count)
        *dir_count = dirs.size();
    // TODO: maybe support sorting by size?
    std::sort(dirs.begin(), dirs.end(), [] (const auto& l, const auto& r) { return l.abs_path() < r.abs_path(); });
    std::sort(files.begin(), files.end(), [] (const auto& l, const auto& r) { return l.abs_path() < r.abs_path(); });

    for (auto& dir : dirs)
        vec.push_back(std::move(dir));

    for (auto& file : files)
        vec.push_back(std::move(file));
}

static void display_file_info(view::Terminal_window& window, const fs::Inode &inode)
{
    window.print_left(1, "NAME: ", COLOR_PAIR(3));
    window.print(inode.name(), A_UNDERLINE);

    window.print_left(3, "SIZE: ", COLOR_PAIR(3));
    window.print(inode.size() + "[Bytes]", A_UNDERLINE);

    window.print_left(5, "RIGHTS: ", COLOR_PAIR(3));
    window.print( inode.rights(), A_UNDERLINE);

    window.print_left(7, "LAST ACCESSED: ", COLOR_PAIR(3));
    window.print(inode.last_accessed(), A_UNDERLINE);

    window.print_left(9, "LAST MODIFIED: ", COLOR_PAIR(3));
    window.print(inode.last_modified(), A_UNDERLINE);

    window.print_left(11, "LAST CHANGED: ", COLOR_PAIR(3));
    window.print( inode.last_status_changed(), A_UNDERLINE);

    window.print_left(13, "INODE: ", COLOR_PAIR(3));
    window.print(inode.inode_number(), A_UNDERLINE);

    window.print_left(15, "HARD LINKS: ", COLOR_PAIR(3));
    window.print(inode.hard_link_count(), A_UNDERLINE);

    if (inode.is_symbolic_link())
    {
        window.print_left(17, "[LINK]-> ", COLOR_PAIR(3));
        window.print(inode.real_name(), A_UNDERLINE);
    }
}

static std::string get_cwd()
{
    char buff[PATH_MAX] = {0};
    std::string cwd = "";
    if (!getcwd(buff, sizeof(buff)))
        utils::Log::the() << "Can't get the cwd\n";
    else
        cwd = buff;
    return cwd;
}

int main()
{
    // take the name of the current user
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    std::string user = pw->pw_name;
    std::string home = pw->pw_dir;
    utils::Log::set_output(home + "/.felog");
    std::string cwd = get_cwd();

    // Setup the scene
    view::Scene &scene = view::Scene::the();
    scene.add_window(0.8f, 0.5f, 0.0f, 0.0f);
    scene.add_window(0.8f, 0.5f, 0.0f, 0.5f);
    scene.add_window(0.2f, 1.0f, 0.8f, 0.0f);
    scene[LEFT].box(0,0);
    scene[RIGHT].box(0, 0);
    scene[BOTTOM].box(0,0);

    // Explicitly set the inout window
    scene.set_input_window(LEFT);
    keypad(*scene.get_input_window(), true);

    // Create and initialize the nececary data structures
    std::vector<fs::Inode> vec;
    int key = 0;
    std::size_t output_lines;
    std::size_t index = 0;
    std::size_t file_count, dir_count;
    bool do_update = true;
    char time_buf[32] = {0};

    fs::Inode current(cwd);
    load_current(current, vec, &file_count, &dir_count);
    output_lines = calculate_lines(scene[LEFT], vec);

    utils::scrollable_vector<fs::Inode> sv(0, output_lines, vec);
    std::vector<fs::Inode> selection;
    std::time_t time;

    while (key != KEY_END)
    {
        index = sv.index();
        fs::Inode& selected_element = sv[index];
        if (index >= output_lines)
            index = 0;

        if (do_update)
        {
            // Clear the windows and rebox them
            scene.erase();
            scene.rebox();
            // Draw things on the windows
            if (sv.size() != 0)
            {
                for (std::size_t i = 0; i < output_lines && i < sv.size(); ++i)
                {
                    attr_t attr = 0;
                    if (sv.is_selected(i))
                        attr = COLOR_PAIR(2);
                    else if (sv[i].is_directory())
                        attr = COLOR_PAIR(1);
                    else
                        attr = COLOR_PAIR(4);

                    if (i == index)
                        attr |= A_STANDOUT;

                    // +1 because it is a boxed window
                    scene[LEFT].print_left(static_cast<int>(i+1), sv[i].name() + (sv[i].is_symbolic_link() ? "*" : ""), attr);
                    if (!sv[i].is_directory())
                        scene[LEFT].print_right(static_cast<int>(i+1), sv[i].formated_size(), attr);

                }
                if (selected_element.is_regular_file() ||
                    selected_element.is_symbolic_link())
                {
                    display_file_info(scene[RIGHT], selected_element);
                }else if (selected_element.is_directory())
                {
                    std::vector<fs::Inode> other_vec;
                    std::size_t fc, dc;
                    load_current(selected_element, other_vec);
                    if (other_vec.size() != 0)
                    {
                        std::size_t right_lines = std::min(static_cast<std::size_t>(scene[RIGHT].lines() - 2), other_vec.size());
                        for (std::size_t i = 0; i < right_lines; ++i)
                        {
                            attr_t attr = 0;
                            if (other_vec[i].is_directory())
                                attr = COLOR_PAIR(1);
                            else
                                attr = COLOR_PAIR(4);
                            // +1 because it is a boxed window
                            scene[RIGHT].print_left(static_cast<int>(i+1), other_vec[i].name() + (other_vec[i].is_symbolic_link() ? "*" : ""), attr);
                            if (!other_vec[i].is_directory())
                                scene[RIGHT].print_right(static_cast<int>(i+1), other_vec[i].formated_size(), attr);
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
        scene[BOTTOM].print(std::to_string(file_count), COLOR_PAIR(4));
        scene[BOTTOM].print("/", COLOR_PAIR(3));
        scene[BOTTOM].print(std::to_string(dir_count) + " ", COLOR_PAIR(1));
        scene[BOTTOM].print(current.rights(), COLOR_PAIR(5));
        if (!selection.empty())
            scene[BOTTOM].print(" selection: " + std::to_string(selection.size()));
        scene[BOTTOM].print_left(2, current.abs_path(), A_UNDERLINE | COLOR_PAIR(3));
        scene[BOTTOM].print_center(3, "[c]reate [d]elete [m]ove [s]elect [e]nd");

        // Refresh the windowws and wait for an event
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
                    if (sv.size() != 0 && selected_element.is_directory())
                    {
                        if (sv.selection_in_progress())
                            sv.interupt_selection();
                        current = selected_element;
                        load_current(current, vec, &file_count, &dir_count);
                        output_lines = calculate_lines(scene[LEFT], vec);
                        sv.reset(0, output_lines, vec);
                    }
                    break;
                case KEY_LEFT:
                {
                    if (current.name() == "/" && current.parent() == "")
                        break;

                    if (sv.selection_in_progress())
                        sv.interupt_selection();

                    current = current.parent_node();
                    load_current(current, vec, &file_count, &dir_count);
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
                        if (ret == 0 && !current.create_file(name))
                            utils::Log::the() << "Failed to create the file: " << name << "\n";
                        else if (ret == 1 && !current.create_dir(name))
                            utils::Log::the() << "Failed to create the directory: " << name << "\n";
                        current.stat();
                        load_current(current, vec, &file_count, &dir_count);
                        output_lines = calculate_lines(scene[LEFT], vec);
                        sv.reset(0, output_lines, vec);
                    }
                    break;
                }
                case 'd':
                    if (selection.empty())
                        break;

                    for (auto& element : selection)
                    {
                        if (!element.remove())
                            utils::Log::the() << "Failed to delete: " << element.abs_path()
                                              << " from " << current.abs_path() << "\n";
                    }
                    current.stat();
                    load_current(current, vec, &file_count, &dir_count);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                    selection.clear();
                    break;
                case 'm':
                    if (selection.empty())
                        break;

                    for (auto& element : selection)
                    {
                        if (!element.move(current))
                            utils::Log::the() << "Failed to move: " << element.abs_path()
                                              << " -> " << current.abs_path() << "\n";
                    }
                    current.stat();
                    load_current(current, vec, &file_count, &dir_count);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                    selection.clear();
                    break;
                case 'p':
                    if (selection.empty())
                        break;

                    for (auto& element : selection)
                    {
                        if (!element.copy(current))
                            utils::Log::the() << "Failed to copy: " << element.abs_path()
                                              << " -> " << current.abs_path() << "\n";
                    }
                    current.stat();
                    load_current(current, vec, &file_count, &dir_count);
                    output_lines = calculate_lines(scene[LEFT], vec);
                    sv.reset(0, output_lines, vec);
                    selection.clear();
                    break;
                case 's':
                    selection.clear();
                    sv.start_selection();
                    break;
                case 'e':
                    selection = sv.end_selection();
                    break;
            }
        }
    }
    return 0;
}
