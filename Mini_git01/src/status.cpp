#include "status.h"
#include "repository.h"
#include <fstream>
#include <iostream>
#include <string>

static std::string getCurrentBranch()
{
    std::ifstream head(".mygit/HEAD");
    std::string ref;
    std::getline(head, ref);

    // ref format: refs/branches/<branch>
    auto pos = ref.find_last_of('/');
    if (pos == std::string::npos)
        return "unknown";
    return ref.substr(pos + 1);
}

static void showStagedFiles()
{
    std::ifstream index(".mygit/index");
    std::string line;
    bool empty = true;

    while (std::getline(index, line))
    {
        if (line.empty())
            continue;
        if (empty)
        {
            std::cout << "Staged files:\n";
            empty = false;
        }
        std::cout << "  " << line << "\n";
    }

    if (empty)
    {
        std::cout << "No files staged.\n";
    }
}

void Status::show()
{
    if (!Repository::exists())
    {
        std::cout << "Not a mygit repository.\n";
        return;
    }

    std::string branch = getCurrentBranch();

    std::cout << "On branch " << branch << "\n\n";

    showStagedFiles();
}
