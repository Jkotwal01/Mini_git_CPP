#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "commit.hpp" // Include the full definition

namespace fs = std::filesystem;

class BranchManager
{
private:
    fs::path repoPath;
    fs::path refsPath;
    fs::path headFile;

    std::string getCurrentBranchName();
    void setCurrentBranch(const std::string &branchName);
    std::string readHead();
    void writeHead(const std::string &content);

public:
    BranchManager(const fs::path &repoPath);

    bool createBranch(const std::string &branchName, const std::string &startPoint = "");
    bool deleteBranch(const std::string &branchName);
    std::vector<std::string> listBranches();
    bool checkoutBranch(const std::string &branchName, CommitManager &commitManager);
    std::string getBranchCommit(const std::string &branchName);
    void setBranchCommit(const std::string &branchName, const std::string &commitId);
    std::string getCurrentBranch();
};

#endif
