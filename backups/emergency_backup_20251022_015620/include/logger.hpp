#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>
#include <iostream>

struct CommitInfo
{
    std::string id;
    std::string message;
    std::string timestamp;
    std::string parentId;
};

class Logger
{
public:
    static void logSuccess(const std::string &message);
    static void logError(const std::string &message);
    static void logInfo(const std::string &message);
    static void displayCommitHistory(const std::vector<CommitInfo> &commits);
    static void displayBranches(const std::vector<std::string> &branches, const std::string &currentBranch);
};

#endif
