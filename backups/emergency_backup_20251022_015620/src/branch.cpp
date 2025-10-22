#include "../include/branch.hpp"
#include "../include/utils.hpp"
#include "../include/logger.hpp"
#include <fstream>

BranchManager::BranchManager(const fs::path &repoPath)
    : repoPath(repoPath),
      refsPath(repoPath / ".minigit" / "refs"),
      headFile(repoPath / ".minigit" / "HEAD")
{
}

std::string BranchManager::getCurrentBranchName()
{
    std::string headContent = readHead();
    if (headContent.find("ref: refs/heads/") == 0)
    {
        return headContent.substr(16); // Remove "ref: refs/heads/"
    }
    return ""; // Detached HEAD
}

void BranchManager::setCurrentBranch(const std::string &branchName)
{
    writeHead("ref: refs/heads/" + branchName);
}

std::string BranchManager::readHead()
{
    if (!fs::exists(headFile))
    {
        return "";
    }
    return Utils::readFile(headFile);
}

void BranchManager::writeHead(const std::string &content)
{
    Utils::writeFile(headFile, content);
}

bool BranchManager::createBranch(const std::string &branchName, const std::string &startPoint)
{
    try
    {
        fs::path branchFile = refsPath / "heads" / branchName;
        if (fs::exists(branchFile))
        {
            Logger::logError("Branch already exists: " + branchName);
            return false;
        }

        std::string commitId = startPoint.empty() ? Utils::readFile(repoPath / ".minigit" / "HEAD") : startPoint;

        if (commitId.find("ref: ") == 0)
        {
            // Resolve branch reference
            fs::path refFile = repoPath / ".minigit" / commitId.substr(5);
            if (fs::exists(refFile))
            {
                commitId = Utils::readFile(refFile);
            }
        }

        fs::create_directories(branchFile.parent_path());
        Utils::writeFile(branchFile, commitId);

        Logger::logSuccess("Created branch " + branchName);
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::logError("Failed to create branch: " + std::string(e.what()));
        return false;
    }
}

bool BranchManager::deleteBranch(const std::string &branchName)
{
    try
    {
        fs::path branchFile = refsPath / "heads" / branchName;
        if (!fs::exists(branchFile))
        {
            Logger::logError("Branch not found: " + branchName);
            return false;
        }

        if (getCurrentBranchName() == branchName)
        {
            Logger::logError("Cannot delete current branch");
            return false;
        }

        fs::remove(branchFile);
        Logger::logSuccess("Deleted branch " + branchName);
        return true;
    }
    catch (...)
    {
        Logger::logError("Failed to delete branch: " + branchName);
        return false;
    }
}

std::vector<std::string> BranchManager::listBranches()
{
    std::vector<std::string> branches;

    fs::path headsPath = refsPath / "heads";
    if (!fs::exists(headsPath))
    {
        return branches;
    }

    for (const auto &entry : fs::directory_iterator(headsPath))
    {
        if (entry.is_regular_file())
        {
            branches.push_back(entry.path().filename().string());
        }
    }

    return branches;
}

bool BranchManager::checkoutBranch(const std::string &branchName, CommitManager &commitManager)
{
    try
    {
        fs::path branchFile = refsPath / "heads" / branchName;
        if (!fs::exists(branchFile))
        {
            Logger::logError("Branch not found: " + branchName);
            return false;
        }

        std::string commitId = Utils::readFile(branchFile);
        if (!commitManager.checkoutCommit(commitId))
        {
            return false;
        }

        setCurrentBranch(branchName);
        Logger::logSuccess("Switched to branch '" + branchName + "'");
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::logError("Checkout failed: " + std::string(e.what()));
        return false;
    }
}

std::string BranchManager::getBranchCommit(const std::string &branchName)
{
    fs::path branchFile = refsPath / "heads" / branchName;
    if (fs::exists(branchFile))
    {
        return Utils::readFile(branchFile);
    }
    return "";
}

void BranchManager::setBranchCommit(const std::string &branchName, const std::string &commitId)
{
    fs::path branchFile = refsPath / "heads" / branchName;
    Utils::writeFile(branchFile, commitId);
}

std::string BranchManager::getCurrentBranch()
{
    return getCurrentBranchName();
}