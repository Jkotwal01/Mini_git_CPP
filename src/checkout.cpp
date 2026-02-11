#include "checkout.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

static bool indexIsEmpty()
{
    std::ifstream in(".mygit/index");
    return in.peek() == std::ifstream::traits_type::eof();
}

static std::string getBranchCommit(const std::string &branch)
{
    std::ifstream in(".mygit/branches/" + branch);
    std::string commit;
    std::getline(in, commit);
    return commit;
}

static std::string getCurrentBranch()
{
    std::ifstream head(".mygit/HEAD");
    std::string ref;
    std::getline(head, ref);
    return ref.substr(ref.find_last_of('/') + 1);
}

// Deletes only files that belong to a commit snapshot
static void deleteTrackedFiles(const std::string &commitID)
{
    if (commitID == "NONE" || commitID.empty())
        return;

    fs::path filesPath = ".mygit/objects/" + commitID + "/files";

    if (!fs::exists(filesPath))
        return;

    for (auto &entry : fs::recursive_directory_iterator(filesPath))
    {
        if (entry.is_regular_file())
        {
            fs::path rel = fs::relative(entry.path(), filesPath);

            if (fs::exists(rel))
                fs::remove(rel);
        }
    }
}

static void restoreSnapshot(const std::string &commitID)
{
    fs::path filesPath = ".mygit/objects/" + commitID + "/files";

    if (!fs::exists(filesPath))
        return;

    for (auto &entry : fs::recursive_directory_iterator(filesPath))
    {
        if (entry.is_regular_file())
        {
            fs::path rel = fs::relative(entry.path(), filesPath);
            if (!rel.parent_path().empty())
                fs::create_directories(rel.parent_path());
            fs::copy_file(entry.path(), rel, fs::copy_options::overwrite_existing);
        }
    }
}



void Checkout::switchBranch(const std::string &branch)
{
    // Step 1 — Check branch exists
    if (!fs::exists(".mygit/branches/" + branch))
    {
        std::cout << "Branch '" << branch << "' does not exist.\n";
        return;
    }

    // Step 2 — Prevent data loss
    if (!indexIsEmpty())
    {
        std::cout << "You have staged changes. Commit or clear them before checkout.\n";
        return;
    }

    // Step 3 — Identify old and new commits
    std::string currentBranch = getCurrentBranch();
    std::string oldCommit = getBranchCommit(currentBranch);
    std::string newCommit = getBranchCommit(branch);

    // Step 4 — Delete only files tracked by current commit
    deleteTrackedFiles(oldCommit);

    // Step 5 — Restore new branch snapshot
    if (newCommit != "NONE")
    {
        restoreSnapshot(newCommit);
    }

    // Step 6 — Update HEAD
    std::ofstream head(".mygit/HEAD");
    head << "ref: refs/branches/" << branch;

    // Step 7 — Notify
    std::cout << "Switched to branch '" << branch << "'\n";
}
