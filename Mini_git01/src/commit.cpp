#include "commit.h"
#include "repository.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;

static std::string generateCommitID()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return std::to_string(time);
}

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
    return commit.empty() ? "NONE" : commit;
}

static void updateBranchHead(const std::string &branch, const std::string &commitID)
{
    std::ofstream out(".mygit/branches/" + branch);
    out << commitID;
}

void Commit::create(const std::string &message)
{
    if (!Repository::exists())
    {
        std::cout << "Not a mygit repository.\n";
        return;
    }

    std::ifstream index(".mygit/index");
    if (index.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "No files staged for commit.\n";
        return;
    }

    std::string commitID = generateCommitID();
    std::string branch = getCurrentBranch();
    std::string parent = getBranchHeadCommit(branch);

    fs::path commitPath = ".mygit/objects/" + commitID;
    fs::create_directory(commitPath);
    fs::create_directories(commitPath / "files");

    // Copy staged files
    std::string file;
    while (std::getline(index, file))
    {
        if (file.empty())
            continue;

        fs::path src = file;
        fs::path dest = commitPath / "files" / file;

        fs::create_directories(dest.parent_path());
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }

    // Write metadata
    std::ofstream meta(commitPath / "meta");
    meta << "commit " << commitID << "\n";
    meta << "parent " << parent << "\n";
    meta << "branch " << branch << "\n";
    meta << "message " << message << "\n";
    meta.close();

    // Update branch head
    updateBranchHead(branch, commitID);

    // Append to log
    std::ofstream log(".mygit/logs/commits.log", std::ios::app);
    log << commitID << " [" << branch << "] " << message << "\n";

    // Clear index
    std::ofstream clear(".mygit/index", std::ios::trunc);

    std::cout << "Committed as " << commitID << "\n";
}
