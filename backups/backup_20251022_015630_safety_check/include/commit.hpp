#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "file_manager.hpp"

class BranchManager; // Forward declaration

namespace fs = std::filesystem;

struct CommitData
{
    std::string id;
    std::string message;
    std::string timestamp;
    std::string parentId;
    std::vector<std::string> parentIds;
    std::map<std::string, std::string> files; // filename -> hash
};

class CommitManager
{
private:
    fs::path repoPath;
    fs::path commitsPath;
    fs::path headFile;
    FileManager &fileManager;

    std::string getHeadCommit();
    void setHeadCommit(const std::string &commitId);
    void writeCommit(const CommitData &commit);

public:
    CommitManager(const fs::path &repoPath, FileManager &fm);
    CommitData readCommit(const std::string &commitId); // Moved to public
    std::string createCommit(const std::string &message, const std::vector<std::string> &parentIds = {});
    std::vector<CommitData> getCommitHistory();
    bool checkoutCommit(const std::string &commitId);
    std::string getCurrentCommitId();
};

#endif
