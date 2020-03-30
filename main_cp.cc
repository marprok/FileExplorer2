#include "inc/fs_node.h"
#include "inc/Directory.h"

int main()
{
    fs::FS_Node* root = new fs::FS_Node(new fs::Directory("/home/void"), nullptr);
    auto print = [](fs::FS_Node* node)
    {
        fs::FS_Node* dir = nullptr;

        std::cout << "\n\n------------printing: " << node->inode()->abs_path() << "------------" << std::endl;
        node->load();

        auto temp = node->dirs().head();
        std::cout << node->dirs().size() << " dirs and " << node->files().size() << " files" << std::endl;
        std::cout << "\n\ndirs" << std::endl;

        while (temp)
        {
            if (temp->data()->inode()->name() == "Src")
            {
                dir = temp->data();
            }
            std::cout << temp->data()->inode()->name() << std::endl;
            temp = temp->next();
        }


        temp = node->files().head();
        std::cout << "\n\nfiles" << std::endl;
        while (temp)
        {
            if (temp->data()->inode()->name() == "cures_example")
                dir = temp->data();
            if (temp->data()->inode()->is_symbolic_link())
                std::cout << "LINK: " << temp->data()->inode()->real_name() << std::endl;
            else
                std::cout << temp->data()->inode()->name() << std::endl;
            temp = temp->next();
        }

        return dir;
    };

    fs::FS_Node* src = print(root);
    fs::FS_Node* cures_example = print(src);
    if (!cures_example)
        std::cout << "NULL" << std::endl;
    std::cout << "cures_example abs path: " << cures_example->abs_path() << std::endl;
    cures_example->move(root);
    std::cout << "cures_example abs path after move : " << cures_example->abs_path() << std::endl;
    std::cout << "\n\n------------after move------------" << std::endl;
    print(root);
    print(src);
    auto tmp = root->dirs().unlink(src);
    delete tmp;
    print(root);


    delete root;
    return 0;
}
