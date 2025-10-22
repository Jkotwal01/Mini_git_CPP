#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager
{
private:
    fs::path repoPath;
    fs::path stagingPath;
    fs::path objectsPath;
    fs::path indexFile;

    std::map<std::string, std::string> readIndex();
    void writeIndex(const std::map<std::string, std::string> &index);

public:
    FileManager(const fs::path &repoPath);

    bool stageFile(const std::string &filename);
    bool unstageFile(const std::string &filename);
    std::vector<std::string> getStagedFiles();
    std::string getFileHash(const fs::path &filepath);
    bool saveFileToObjects(const std::string &content, const std::string &hash);
    std::string getFileFromObjects(const std::string &hash);
    void clearStaging();
    std::map<std::string, std::string> getStagedFilesWithHashes();
};

#endif
