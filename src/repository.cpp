#include "repository.h"
#include <filesystem>
#include <fstream>
#include <iostream>

// Creates an alias:
// fs becomes a short name for std::filesystem.
namespace fs = std::filesystem;

bool Repository::exists()
{
    return fs::exists(".mygit") && fs::is_directory(".mygit");
}

void Repository::init()
{
    if (exists())
    {
        std::cout << "Repository already initialized.\n";
        return;
    }

    fs::create_directory(".mygit");
    fs::create_directory(".mygit/objects");
    fs::create_directory(".mygit/branches");
    fs::create_directory(".mygit/logs");

    // HEAD points to main branch
    std::ofstream head(".mygit/HEAD");
    head << "refs/branches/main";
    head.close();

    // main branch (empty commit pointer)
    std::ofstream mainBranch(".mygit/branches/main");
    mainBranch.close();

    // staging area
    std::ofstream index(".mygit/index");
    index.close();

    std::cout << "Initialized empty repository.\n";
}
