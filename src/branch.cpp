#include "branch.h"
#include <filesystem>
#include <fstream>
#include <iostream>

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

void Branch::create(const std::string &name)
{
    if (!fs::exists(".mygit"))
    {
        std::cout << "Not a mygit repository.\n";
        return;
    }

    fs::path branchPath = ".mygit/branches/" + name;

    if (fs::exists(branchPath))
    {
        std::cout << "Branch '" << name << "' already exists.\n";
        return;
    }

    std::string current = getCurrentBranch();
    std::string headCommit = getBranchHeadCommit(current);

    std::ofstream out(branchPath);
    out << headCommit;
    out.close();

    std::cout << "Branch '" << name << "' created at " << headCommit << "\n";
}

void Branch::list()
{
    if (!fs::exists(".mygit/branches"))
    {
        std::cout << "No branches found.\n";
        return;
    }

    std::string current = getCurrentBranch();

    for (const auto &entry : fs::directory_iterator(".mygit/branches"))
    {
        std::string name = entry.path().filename().string();

        if (name == current)
            std::cout << "* ";
        else
            std::cout << "  ";

        std::cout << name << "\n";
    }
}
