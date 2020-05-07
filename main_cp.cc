#include "inode.h"
#include <vector>
#include <iostream>

int main(void)
{
    fs::Inode inode("", "/");
    auto parent = inode.parent_node();
    std::cout << "parent: " << inode.parent() << " name: " << inode.name() << std::endl;
    std::cout << "parent: " << parent.parent() << " name: " << parent.name() << std::endl;

    std::vector<fs::Inode> files, dirs;

    inode.load(files, dirs);

    std::cout << "files:" << std::endl;
    for (auto& file : files)
        std::cout << "parent: " << file.parent() << " name: " << file.name() << std::endl;
    std::cout << "dirs:" << std::endl;
    for (auto& dir : dirs)
        std::cout << "parent: " << dir.parent() << " name: " << dir.name() << std::endl;

    return 0;
}
