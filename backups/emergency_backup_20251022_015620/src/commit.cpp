#include "../include/commit.hpp"
#include "../include/utils.hpp"
#include "../include/logger.hpp"
#include <fstream>
#include <sstream>

CommitManager::CommitManager(const fs::path &repoPath, FileManager &fm)
    : repoPath(repoPath),
      commitsPath(repoPath / ".minigit" / "commits"),
      headFile(repoPath / ".minigit" / "HEAD"),
      fileManager(fm)
{
}

std::string CommitManager::getHeadCommit()
{
    if (!fs::exists(headFile))
    {
        return "";
    }

    std::string content = Utils::readFile(headFile);
    // Check if HEAD points to a branch
    if (content.find("ref: refs/") == 0)
    {
        fs::path branchFile = repoPath / ".minigit" / content.substr(5);
        if (fs::exists(branchFile))
        {
            return Utils::readFile(branchFile);
        }
    }
    return content; // Direct commit reference
}

void CommitManager::setHeadCommit(const std::string &commitId)
{
    Utils::writeFile(headFile, commitId);
}

CommitData CommitManager::readCommit(const std::string &commitId)
{
    CommitData commit;
    commit.id = commitId;

    fs::path commitFile = commitsPath / commitId;
    if (!fs::exists(commitFile))
    {
        return commit;
    }

    std::string content = Utils::readFile(commitFile);
    auto lines = Utils::splitString(content, '\n');

    for (const auto &line : lines)
    {
        if (line.find("parent:") == 0)
        {
            commit.parentId = line.substr(8);
            commit.parentIds.push_back(commit.parentId);
        }
        else if (line.find("parents:") == 0)
        {
            auto parents = Utils::splitString(line.substr(9), ',');
            commit.parentIds = parents;
        }
        else if (line.find("message:") == 0)
        {
            commit.message = line.substr(9);
        }
        else if (line.find("timestamp:") == 0)
        {
            commit.timestamp = line.substr(11);
        }
        else if (line.find("file:") == 0)
        {
            auto parts = Utils::splitString(line.substr(6), '|');
            if (parts.size() == 2)
            {
                commit.files[parts[0]] = parts[1];
            }
        }
    }

    return commit;
}

void CommitManager::writeCommit(const CommitData &commit)
{
    fs::path commitFile = commitsPath / commit.id;
    fs::create_directories(commitFile.parent_path());

    std::stringstream content;
    content << "message: " << commit.message << "\n";
    content << "timestamp: " << commit.timestamp << "\n";

    if (commit.parentIds.size() == 1)
    {
        content << "parent: " << commit.parentIds[0] << "\n";
    }
    else if (commit.parentIds.size() > 1)
    {
        content << "parents: " << Utils::joinStrings(commit.parentIds, ",") << "\n";
    }

    for (const auto &[filename, hash] : commit.files)
    {
        content << "file: " << filename << "|" << hash << "\n";
    }

    Utils::writeFile(commitFile, content.str());
}

std::string CommitManager::createCommit(const std::string &message, const std::vector<std::string> &parentIds)
{
    try
    {
        auto stagedFiles = fileManager.getStagedFilesWithHashes();
        if (stagedFiles.empty())
        {
            Logger::logError("No files staged for commit");
            return "";
        }

        CommitData commit;
        commit.message = message;
        commit.timestamp = Utils::getCurrentTimestamp();
        commit.parentIds = parentIds;
        commit.files = stagedFiles;

        // Generate commit ID from content
        std::stringstream commitContent;
        commitContent << message << commit.timestamp;
        for (const auto &parentId : parentIds)
        {
            commitContent << parentId;
        }
        for (const auto &[filename, hash] : stagedFiles)
        {
            commitContent << filename << hash;
        }
        commit.id = Utils::generateHash(commitContent.str());

        // Save files to objects
        for (const auto &[filename, hash] : stagedFiles)
        {
            fs::path sourceFile = repoPath / ".minigit" / "staging" / filename;
            if (fs::exists(sourceFile))
            {
                std::string fileContent = Utils::readFile(sourceFile);
                fileManager.saveFileToObjects(fileContent, hash);
            }
        }

        // Write commit metadata
        writeCommit(commit);

        // Update HEAD
        setHeadCommit(commit.id);

        // Clear staging area
        fileManager.clearStaging();

        Logger::logSuccess("Created commit " + commit.id.substr(0, 8) + ": " + message);
        return commit.id;
    }
    catch (const std::exception &e)
    {
        Logger::logError("Failed to create commit: " + std::string(e.what()));
        return "";
    }
}

std::vector<CommitData> CommitManager::getCommitHistory()
{
    std::vector<CommitData> commits;
    std::string currentCommitId = getHeadCommit();

    while (!currentCommitId.empty())
    {
        CommitData commit = readCommit(currentCommitId);
        if (commit.id.empty())
            break;

        commits.push_back(commit);
        currentCommitId = commit.parentId; // Follow first parent for linear history
    }

    return commits;
}

bool CommitManager::checkoutCommit(const std::string &commitId)
{
    try
    {
        CommitData commit = readCommit(commitId);
        if (commit.id.empty())
        {
            Logger::logError("Commit not found: " + commitId);
            return false;
        }

        // Clear working directory (except .minigit)
        for (const auto &entry : fs::directory_iterator(repoPath))
        {
            if (entry.path().filename() != ".minigit")
            {
                if (fs::is_directory(entry))
                {
                    fs::remove_all(entry);
                }
                else
                {
                    fs::remove(entry);
                }
            }
        }

        // Restore files from commit
        for (const auto &[filename, hash] : commit.files)
        {
            std::string content = fileManager.getFileFromObjects(hash);
            if (!content.empty())
            {
                fs::path filePath = repoPath / filename;
                fs::create_directories(filePath.parent_path());
                Utils::writeFile(filePath, content);
            }
        }

        // Update HEAD to point directly to the commit (detached HEAD)
        setHeadCommit(commitId);

        Logger::logSuccess("Checked out commit " + commitId.substr(0, 8));
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::logError("Checkout failed: " + std::string(e.what()));
        return false;
    }
}

std::string CommitManager::getCurrentCommitId()
{
    return getHeadCommit();
}