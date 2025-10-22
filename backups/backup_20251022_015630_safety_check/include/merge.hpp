#ifndef MERGE_HPP
#define MERGE_HPP

#include <string>
#include <map>
#include <filesystem>
#include "commit.hpp"
#include "branch.hpp"

namespace fs = std::filesystem;

struct MergeResult
{
    bool success;
    bool hasConflicts;
    std::vector<std::string> conflictFiles;
};

class MergeManager
{
private:
    fs::path repoPath;
    FileManager &fileManager;
    CommitManager &commitManager;
    BranchManager &branchManager;

    std::string findCommonAncestor(const std::string &commitId1, const std::string &commitId2);
    std::map<std::string, std::string> getCommitFiles(const std::string &commitId);
    void createConflictFile(const fs::path &filepath,
                            const std::string &currentContent,
                            const std::string &otherContent);

public:
    MergeManager(const fs::path &repoPath, FileManager &fm, CommitManager &cm, BranchManager &bm);

    MergeResult mergeBranch(const std::string &branchName);
};

#endif
