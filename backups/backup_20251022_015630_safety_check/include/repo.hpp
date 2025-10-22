#ifndef REPO_HPP
#define REPO_HPP

#include <filesystem>
#include "file_manager.hpp"
#include "commit.hpp"
#include "branch.hpp"
#include "merge.hpp"

namespace fs = std::filesystem;

class Repository
{
private:
    fs::path repoPath;
    FileManager fileManager;
    CommitManager commitManager;
    BranchManager branchManager;
    MergeManager mergeManager;

    bool isInitialized();

public:
    Repository();

    bool init();
    bool add(const std::string &filename);
    bool commit(const std::string &message);
    void log();
    bool checkout(const std::string &target);
    bool createBranch(const std::string &branchName);
    void listBranches();
    bool merge(const std::string &branchName);
    std::string getStatus();
};

#endif
