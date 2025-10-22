#include "../include/merge.hpp"
#include "../include/utils.hpp"
#include "../include/logger.hpp"
#include <set>

MergeManager::MergeManager(const fs::path &repoPath, FileManager &fm, CommitManager &cm, BranchManager &bm)
    : repoPath(repoPath), fileManager(fm), commitManager(cm), branchManager(bm)
{
}

std::string MergeManager::findCommonAncestor(const std::string &commitId1, const std::string &commitId2)
{
    // Simple implementation: traverse parents to find common ancestor
    // In a real implementation, you'd use a more sophisticated algorithm

    std::set<std::string> visited;
    std::string current = commitId1;

    while (!current.empty())
    {
        visited.insert(current);
        auto commit = commitManager.readCommit(current);
        current = commit.parentId;
    }

    current = commitId2;
    while (!current.empty())
    {
        if (visited.find(current) != visited.end())
        {
            return current;
        }
        auto commit = commitManager.readCommit(current);
        current = commit.parentId;
    }

    return ""; // No common ancestor found
}

std::map<std::string, std::string> MergeManager::getCommitFiles(const std::string &commitId)
{
    auto commit = commitManager.readCommit(commitId);
    return commit.files;
}

void MergeManager::createConflictFile(const fs::path &filepath,
                                      const std::string &currentContent,
                                      const std::string &otherContent)
{
    std::stringstream conflictContent;
    conflictContent << "<<<<<<< HEAD\n";
    conflictContent << currentContent;
    conflictContent << "=======\n";
    conflictContent << otherContent;
    conflictContent << ">>>>>>> merged\n";

    fs::create_directories(filepath.parent_path());
    Utils::writeFile(filepath, conflictContent.str());
}

MergeResult MergeManager::mergeBranch(const std::string &branchName)
{
    MergeResult result;
    result.success = false;
    result.hasConflicts = false;

    try
    {
        std::string currentBranch = branchManager.getCurrentBranch();
        if (currentBranch.empty())
        {
            Logger::logError("Cannot merge in detached HEAD state");
            return result;
        }

        if (currentBranch == branchName)
        {
            Logger::logError("Cannot merge branch with itself");
            return result;
        }

        std::string currentCommitId = branchManager.getBranchCommit(currentBranch);
        std::string otherCommitId = branchManager.getBranchCommit(branchName);

        if (otherCommitId.empty())
        {
            Logger::logError("Branch not found: " + branchName);
            return result;
        }

        std::string commonAncestorId = findCommonAncestor(currentCommitId, otherCommitId);
        if (commonAncestorId.empty())
        {
            Logger::logError("No common ancestor found");
            return result;
        }

        auto ancestorFiles = getCommitFiles(commonAncestorId);
        auto currentFiles = getCommitFiles(currentCommitId);
        auto otherFiles = getCommitFiles(otherCommitId);

        // Collect all unique files
        std::set<std::string> allFiles;
        for (const auto &[file, hash] : ancestorFiles)
            allFiles.insert(file);
        for (const auto &[file, hash] : currentFiles)
            allFiles.insert(file);
        for (const auto &[file, hash] : otherFiles)
            allFiles.insert(file);

        bool hasConflicts = false;

        // Stage files for merge commit
        for (const auto &filename : allFiles)
        {
            std::string ancestorHash = ancestorFiles.count(filename) ? ancestorFiles[filename] : "";
            std::string currentHash = currentFiles.count(filename) ? currentFiles[filename] : "";
            std::string otherHash = otherFiles.count(filename) ? otherFiles[filename] : "";

            // Three-way merge logic
            if (currentHash == otherHash)
            {
                // Files are identical in both branches - no action needed
                continue;
            }
            else if (ancestorHash == currentHash)
            {
                // File changed in other branch but not in current - take other branch's version
                std::string content = fileManager.getFileFromObjects(otherHash);
                if (!content.empty())
                {
                    fs::path filePath = repoPath / filename;
                    Utils::writeFile(filePath, content);
                    fileManager.stageFile(filename);
                }
            }
            else if (ancestorHash == otherHash)
            {
                // File changed in current branch but not in other - keep current version
                // No action needed as we're already on current branch
                continue;
            }
            else if (currentHash != otherHash)
            {
                // Conflict - both branches modified the file differently
                hasConflicts = true;
                result.conflictFiles.push_back(filename);

                std::string currentContent = fileManager.getFileFromObjects(currentHash);
                std::string otherContent = fileManager.getFileFromObjects(otherHash);

                createConflictFile(repoPath / filename, currentContent, otherContent);
                Logger::logInfo("Conflict in file: " + filename);
            }
        }

        if (hasConflicts)
        {
            result.hasConflicts = true;
            Logger::logError("Merge conflicts detected. Please resolve them and commit.");
            return result;
        }

        // Create merge commit
        std::vector<std::string> parentIds = {currentCommitId, otherCommitId};
        std::string mergeCommitId = commitManager.createCommit("Merge branch '" + branchName + "'", parentIds);

        if (!mergeCommitId.empty())
        {
            result.success = true;
            branchManager.setBranchCommit(currentBranch, mergeCommitId);
            Logger::logSuccess("Successfully merged branch '" + branchName + "'");
        }
    }
    catch (const std::exception &e)
    {
        Logger::logError("Merge failed: " + std::string(e.what()));
    }

    return result;
}