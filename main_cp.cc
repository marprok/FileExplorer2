#include "inc/node.h"
#include "inc/directory.h"
#include "inc/linked_list.hpp"

int main()
{
    fs::Node* root = new fs::Node(new fs::Directory("/home/void/Desktop/dirs"), nullptr);

    root->load();

    auto head = root->dirs().head();
    fs::Node* dirs = nullptr;
    while (head)
    {
        if (head->data()->inode()->name() == "dir1")
            dirs = head->data();
        head = head->next();
    }

    if (dirs)
        root->remove(dirs);

    delete root;
    return 0;
}
