#include "../include/file_manager.hpp"
#include "../include/utils.hpp"
#include <fstream>
#include <sstream>

FileManager::FileManager(const fs::path &repoPath)
    : repoPath(repoPath),
      stagingPath(repoPath / ".minigit" / "staging"),
      objectsPath(repoPath / ".minigit" / "objects"),
      indexFile(repoPath / ".minigit" / "index.txt")
{
}

std::map<std::string, std::string> FileManager::readIndex()
{
    std::map<std::string, std::string> index;

    if (!fs::exists(indexFile))
    {
        return index;
    }

    std::ifstream file(indexFile);
    std::string line;

    while (std::getline(file, line))
    {
        auto parts = Utils::splitString(line, '|');
        if (parts.size() == 2)
        {
            index[parts[0]] = parts[1];
        }
    }

    return index;
}

void FileManager::writeIndex(const std::map<std::string, std::string> &index)
{
    std::ofstream file(indexFile);
    for (const auto &[filename, hash] : index)
    {
        file << filename << "|" << hash << std::endl;
    }
}

bool FileManager::stageFile(const std::string &filename)
{
    try
    {
        fs::path sourcePath = repoPath / filename;
        if (!fs::exists(sourcePath))
        {
            return false;
        }

        // Create staging directory if it doesn't exist
        fs::create_directories(stagingPath);

        // Copy file to staging area
        fs::path destPath = stagingPath / filename;
        fs::create_directories(destPath.parent_path());
        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);

        // Update index
        auto index = readIndex();
        std::string content = Utils::readFile(sourcePath);
        std::string hash = Utils::generateHash(content);
        index[filename] = hash;
        writeIndex(index);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FileManager::unstageFile(const std::string &filename)
{
    try
    {
        auto index = readIndex();
        auto it = index.find(filename);
        if (it != index.end())
        {
            index.erase(it);
            writeIndex(index);

            // Remove from staging area
            fs::path stagedFile = stagingPath / filename;
            if (fs::exists(stagedFile))
            {
                fs::remove(stagedFile);
            }
            return true;
        }
        return false;
    }
    catch (...)
    {
        return false;
    }
}

std::vector<std::string> FileManager::getStagedFiles()
{
    auto index = readIndex();
    std::vector<std::string> files;
    for (const auto &[filename, hash] : index)
    {
        files.push_back(filename);
    }
    return files;
}

std::string FileManager::getFileHash(const fs::path &filepath)
{
    if (!fs::exists(filepath))
    {
        return "";
    }
    std::string content = Utils::readFile(filepath);
    return Utils::generateHash(content);
}

bool FileManager::saveFileToObjects(const std::string &content, const std::string &hash)
{
    try
    {
        fs::path objectPath = objectsPath / hash.substr(0, 2) / hash.substr(2);
        fs::create_directories(objectPath.parent_path());
        Utils::writeFile(objectPath, content);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string FileManager::getFileFromObjects(const std::string &hash)
{
    try
    {
        fs::path objectPath = objectsPath / hash.substr(0, 2) / hash.substr(2);
        if (fs::exists(objectPath))
        {
            return Utils::readFile(objectPath);
        }
        return "";
    }
    catch (...)
    {
        return "";
    }
}

void FileManager::clearStaging()
{
    try
    {
        // Clear staging directory
        if (fs::exists(stagingPath))
        {
            fs::remove_all(stagingPath);
            fs::create_directories(stagingPath);
        }

        // Clear index
        if (fs::exists(indexFile))
        {
            fs::remove(indexFile);
        }
    }
    catch (...)
    {
        // Ignore errors during cleanup
    }
}

std::map<std::string, std::string> FileManager::getStagedFilesWithHashes()
{
    return readIndex();
}