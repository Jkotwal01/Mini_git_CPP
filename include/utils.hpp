#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;

class Utils
{
public:
    static std::string generateHash(const std::string &content);
    static std::string getCurrentTimestamp();
    static std::string readFile(const fs::path &filepath);
    static void writeFile(const fs::path &filepath, const std::string &content);
    static bool copyFile(const fs::path &source, const fs::path &destination);
    static std::vector<std::string> splitString(const std::string &str, char delimiter);
    static std::string joinStrings(const std::vector<std::string> &strings, const std::string &delimiter);
    static bool isFileEqual(const fs::path &file1, const fs::path &file2);
};

#endif
