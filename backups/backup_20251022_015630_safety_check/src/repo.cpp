#include "../include/repo.hpp"
#include "../include/logger.hpp"
#include "../include/utils.hpp" // ✅ Added
#include <algorithm>            // ✅ Added for std::find
#include <sstream>              // ✅ Added
#include <vector>
#include <filesystem>

Repository::Repository()
    : repoPath(fs::current_path()),
      fileManager(repoPath),
      commitManager(repoPath, fileManager),
      branchManager(repoPath),
      mergeManager(repoPath, fileManager, commitManager, branchManager)
{
}

bool Repository::isInitialized()
{
    return fs::exists(repoPath / ".minigit");
}

bool Repository::init()
{
    try
    {
        if (isInitialized())
        {
            Logger::logInfo("MiniGit repository already initialized");
            return true;
        }

        fs::path minigitPath = repoPath / ".minigit";
        fs::create_directories(minigitPath / "objects");
        fs::create_directories(minigitPath / "commits");
        fs::create_directories(minigitPath / "staging");
        fs::create_directories(minigitPath / "refs" / "heads");

        // ✅ Fixed: make sure Utils is declared
        Utils::writeFile(minigitPath / "HEAD", "ref: refs/heads/master");

        // Create initial commit
        std::string initialCommitId = commitManager.createCommit("Initial commit");
        branchManager.setBranchCommit("master", initialCommitId);

        Logger::logSuccess("Initialized empty MiniGit repository in " + (repoPath / ".minigit").string());
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::logError("Initialization failed: " + std::string(e.what()));
        return false;
    }
}

bool Repository::add(const std::string &filename)
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return false;
    }

    if (fileManager.stageFile(filename))
    {
        Logger::logSuccess("Staged " + filename);
        return true;
    }
    else
    {
        Logger::logError("Failed to stage " + filename);
        return false;
    }
}

bool Repository::commit(const std::string &message)
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return false;
    }

    return !commitManager.createCommit(message).empty();
}

void Repository::log()
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return;
    }

    auto commits = commitManager.getCommitHistory();
    std::vector<CommitInfo> commitInfos;

    for (const auto &commit : commits)
    {
        CommitInfo info;
        info.id = commit.id;
        info.message = commit.message;
        info.timestamp = commit.timestamp;
        info.parentId = commit.parentId;
        commitInfos.push_back(info);
    }

    Logger::displayCommitHistory(commitInfos);
}

bool Repository::checkout(const std::string &target)
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return false;
    }

    // ✅ Added <algorithm> to fix this
    auto branches = branchManager.listBranches();
    if (std::find(branches.begin(), branches.end(), target) != branches.end())
    {
        return branchManager.checkoutBranch(target, commitManager);
    }
    else
    {
        return commitManager.checkoutCommit(target);
    }
}

bool Repository::createBranch(const std::string &branchName)
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return false;
    }

    return branchManager.createBranch(branchName);
}

void Repository::listBranches()
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return;
    }

    auto branches = branchManager.listBranches();
    std::string currentBranch = branchManager.getCurrentBranch();
    Logger::displayBranches(branches, currentBranch);
}

bool Repository::merge(const std::string &branchName)
{
    if (!isInitialized())
    {
        Logger::logError("Not a MiniGit repository");
        return false;
    }

    MergeResult result = mergeManager.mergeBranch(branchName);
    return result.success && !result.hasConflicts;
}

std::string Repository::getStatus()
{
    if (!isInitialized())
    {
        return "Not a MiniGit repository";
    }

    std::stringstream status;
    std::string currentBranch = branchManager.getCurrentBranch();
    status << "On branch " << (currentBranch.empty() ? "detached HEAD" : currentBranch) << "\n";

    auto stagedFiles = fileManager.getStagedFiles();
    if (!stagedFiles.empty())
    {
        status << "Staged changes:\n";
        for (const auto &file : stagedFiles)
        {
            status << "  " << file << "\n";
        }
    }
    else
    {
        status << "No staged changes\n";
    }

    return status.str();
}
