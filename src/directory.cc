#include <dirent.h>
#include <iostream>
#include <unistd.h> // rmdir
#include "../inc/file.h"
#include "../inc/directory.h"
#include "../inc/node.h"

namespace fs
{
Directory::Directory(const std::string& name)
    :Inode (name)
{

}

Directory::~Directory()
{

}

std::size_t Directory::populate(Node* node)
{
    DIR *dir = dir = opendir(node->abs_path().c_str());

    struct dirent *drt;

    if (!dir)
    {
        std::cerr << "ERROR: opendir" << std::endl << "Directory abs name: "<< node->abs_path() << std::endl;
        return 0;
    }

    while ((drt = readdir(dir)))
    {
        if (drt->d_type != DT_DIR)
        {
            /* This is not a directory. */
            auto file = new File(drt->d_name);
            node->files().insert_front(new Node(file, node));
        }else
        {
            /* In case the m_name is . or .. */
            size_t temp = strlen(drt->d_name);
            if ((temp == 1 && drt->d_name[0] == '.') ||
                    (temp == 2 && drt->d_name[0] == '.' && drt->d_name[1] == '.'))
            {
                continue;
            }
            auto directory = new Directory(drt->d_name);
            node->dirs().insert_front(new Node(directory, node));
        }
    }

    closedir(dir);
    return node->size();
}

int Directory::remove(const Node *node)
{
    assert(node);
    return rmdir(node->abs_path().c_str());
}
}