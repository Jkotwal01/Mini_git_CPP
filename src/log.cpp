#include "log.h"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

static std::string getCurrentBranch()
{
    std::ifstream head(".mygit/HEAD");
    std::string ref;
    std::getline(head, ref);
    return ref.substr(ref.find_last_of('/') + 1);
}

static std::string getBranchHeadCommit(const std::string &branch)
{
    std::ifstream in(".mygit/branches/" + branch);
    std::string commit;
    std::getline(in, commit);
    return commit;
}

void Log::show()
{
    if (!fs::exists(".mygit"))
    {
        std::cout << "Not a mygit repository.\n";
        return;
    }

    std::string branch = getCurrentBranch();
    std::string commitID = getBranchHeadCommit(branch);

    if (commitID.empty())
    {
        std::cout << "No commits yet.\n";
        return;
    }

    while (commitID != "NONE")
    {
        fs::path metaPath = ".mygit/objects/" + commitID + "/meta";
        std::ifstream meta(metaPath);

        if (!meta.is_open())
        {
            std::cout << "Corrupt commit: " << commitID << "\n";
            break;
        }

        std::string line, parent, message;

        while (std::getline(meta, line))
        {
            if (line.rfind("parent ", 0) == 0)
                parent = line.substr(7);
            else if (line.rfind("message ", 0) == 0)
                message = line.substr(8);
        }

        std::cout << "commit " << commitID << "\n";
        std::cout << "    " << message << "\n\n";

        commitID = parent;
    }
}
