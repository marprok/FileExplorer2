#include "inode.h"
#include <vector>
#include <iostream>

int main(void)
{
    fs::Inode inode("/home/void", "Desktop");

    bool end = false;
    while (!end)
    {
        std::vector<fs::Inode> files, dirs;
        std::cout << inode.abs_path() << std::endl;
        inode.load(files, dirs);
        std::cout << "files:" << std::endl;
        for (auto& file : files)
            std::cout << " name: " << file.name() << std::endl;
        std::cout << "dirs:" << std::endl;
        for (auto& dir : dirs)
            std::cout << " name: " << dir.name() << std::endl;

        std::cout << "\n\n" << std::endl;
        if (inode.name() == "/" && inode.parent() == "")
            end = true;
        inode = inode.parent_node();
    }

    return 0;
}
