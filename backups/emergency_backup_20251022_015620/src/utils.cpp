#include "../include/utils.hpp"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

std::string Utils::generateHash(const std::string &content)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(content.c_str()), content.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string Utils::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Utils::readFile(const fs::path &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filepath.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Utils::writeFile(const fs::path &filepath, const std::string &content)
{
    std::ofstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot write to file: " + filepath.string());
    }
    file << content;
}

bool Utils::copyFile(const fs::path &source, const fs::path &destination)
{
    try
    {
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::vector<std::string> Utils::splitString(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string Utils::joinStrings(const std::vector<std::string> &strings, const std::string &delimiter)
{
    std::string result;
    for (size_t i = 0; i < strings.size(); ++i)
    {
        result += strings[i];
        if (i < strings.size() - 1)
        {
            result += delimiter;
        }
    }
    return result;
}

bool Utils::isFileEqual(const fs::path &file1, const fs::path &file2)
{
    try
    {
        if (!fs::exists(file1) || !fs::exists(file2))
        {
            return false;
        }

        std::string content1 = readFile(file1);
        std::string content2 = readFile(file2);

        return content1 == content2;
    }
    catch (...)
    {
        return false;
    }
}