#include "index.h"
#include "repository.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>

namespace fs = std::filesystem;

static const std::string INDEX_PATH = ".mygit/index";

/*
 * @brief Normalize a filesystem path for storage in the index.
 *
 * Converts the given path into a relative path using forward slashes.
 * This ensures paths are stored consistently across different platforms.
 *
 * @param p The filesystem path to normalize.
 * @return A normalized, relative path as a string.
 */
static std::string normalizePath(const fs::path &p) 
{
    return fs::relative(p).generic_string();
}

/*
 * @brief Check whether a file is already staged.
 *
 * Reads the index file line by line and checks if the given file
 * path already exists in the staging area.
 *
 * @param file The normalized file path to check.
 * @return true if the file is already staged, false otherwise.
 */
bool Index::isStaged(const std::string &file)
{
    std::ifstream in(INDEX_PATH);
    std::string line;
    while (std::getline(in, line))
    {
        if (line == file)
            return true;
    }
    return false;
}

/*
 * @brief Stage a single file.
 *
 * Normalizes the file path and appends it to the index file
 * if it is not already staged.
 *
 * @param file The filesystem path of the file to stage.
 */

static void stageFile(const fs::path &file)
{
    std::string normalized = normalizePath(file);

    if (Index::isStaged(normalized))
        return;

    std::ofstream out(INDEX_PATH, std::ios::app);
    out << normalized << "\n";
}
/*
 * @brief Add files or directories to the staging area.
 *
 * Accepts a list of paths and stages all regular files found.
 * Directories are traversed recursively. Duplicate entries
 * are avoided, and staged files from this operation are
 * reported to the user.
 *
 * @param paths A list of file or directory paths to stage.
 */
void Index::add(const std::vector<std::string> &paths)
{
    if (!Repository::exists())
    {
        std::cout << "Not a mygit repository.\n";
        return;
    }

    std::set<std::string> stagedNow;

    for (const auto &input : paths)
    {
        fs::path p(input);

        if (!fs::exists(p))
        {
            std::cout << "Path does not exist: " << input << "\n";
            continue;
        }

        if (fs::is_regular_file(p))
        {
            stageFile(p);
            stagedNow.insert(normalizePath(p));
        }
        else if (fs::is_directory(p))
        {
            for (const auto &entry : fs::recursive_directory_iterator(p))
            {
                if (entry.is_regular_file())
                {
                    stageFile(entry.path());
                    stagedNow.insert(normalizePath(entry.path()));
                }
            }
        }
    }

    if (!stagedNow.empty())
    {
        std::cout << "Staged files:\n";
        for (const auto &f : stagedNow)
        {
            std::cout << "  " << f << "\n";
        }
    }
}
