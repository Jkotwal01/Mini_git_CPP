#include "../include/logger.hpp"
#include <iomanip>

void Logger::logSuccess(const std::string &message)
{
    std::cout << "\033[32m✓ " << message << "\033[0m" << std::endl;
}

void Logger::logError(const std::string &message)
{
    std::cerr << "\033[31m✗ " << message << "\033[0m" << std::endl;
}

void Logger::logInfo(const std::string &message)
{
    std::cout << "ℹ " << message << std::endl;
}

void Logger::displayCommitHistory(const std::vector<CommitInfo> &commits)
{
    if (commits.empty())
    {
        std::cout << "No commits yet." << std::endl;
        return;
    }

    for (const auto &commit : commits)
    {
        std::cout << "\033[33mcommit " << commit.id.substr(0, 8) << "\033[0m" << std::endl;
        std::cout << "Date:   " << commit.timestamp << std::endl;
        std::cout << "    " << commit.message << std::endl
                  << std::endl;
    }
}

void Logger::displayBranches(const std::vector<std::string> &branches, const std::string &currentBranch)
{
    if (branches.empty())
    {
        std::cout << "No branches yet." << std::endl;
        return;
    }

    for (const auto &branch : branches)
    {
        if (branch == currentBranch)
        {
            std::cout << "\033[32m* " << branch << "\033[0m" << std::endl;
        }
        else
        {
            std::cout << "  " << branch << std::endl;
        }
    }
}