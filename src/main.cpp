#include <iostream>
#include <vector>
#include "repository.h"
#include "help.h"
#include "index.h"
#include"status.h"
#include "commit.h"
#include "log.h"
#include "branch.h"
#include "checkout.h"
#include "diff.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: mygit <command>\n";
        std::cout << "Run 'mygit help' for available commands.\n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "help")
    {
        Help::show();
    }
    else if (command == "init")
    {
        Repository::init();
    }
    else if (command == "add")
    {
        if (argc < 3)
        {
            std::cout << "Usage: mygit add <paths...>\n";
            return 0;
        }

        std::vector<std::string> paths;
        for (int i = 2; i < argc; i++)
        {
            paths.push_back(argv[i]);
        }

        Index::add(paths);
    }
    else if (command == "status")
    {
        Status::show();
    }
    else if (command == "commit")
    {
        if (argc < 3)
        {
            std::cout << "Usage: mygit commit \"message\"\n";
            return 0;
        }
        Commit::create(argv[2]);
    }
    else if (command == "log")
    {
        Log::show();
    }
    else if (command == "branch")
    {
        if (argc == 2)
        {
            Branch::list();
        }
        else
        {
            Branch::create(argv[2]);
        }
    }
    else if (command == "checkout")
    {
        if (argc < 3)
        {
            std::cout << "Usage: mygit checkout <branch>\n";
            return 0;
        }
        Checkout::switchBranch(argv[2]);
    }
    else if (command == "diff")
    {
        if (argc != 4)
        {
            std::cout << "Usage: mygit diff <commitA> <commitB>\n";
            return 0;
        }

        Diff::show(argv[2], argv[3]);
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Run 'mygit help' for available commands.\n";
    }

    return 0;
}
