#include "diff.h"
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

static std::unordered_map<std::string, fs::path>
buildFileMap(const std::string &commit)
{
    std::unordered_map<std::string, fs::path> files;

    fs::path root = ".mygit/objects/" + commit + "/files";

    for (auto &entry : fs::recursive_directory_iterator(root))
    {
        if (entry.is_regular_file())
        {
            fs::path rel = fs::relative(entry.path(), root);
            files[rel.string()] = entry.path();
        }
    }

    return files;
}
// longest common Subsequence.(LCS)
static std::vector<std::string> readLines(const fs::path &file)
{
    std::ifstream in(file);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line))
        lines.push_back(line);

    return lines;
}

// Longest common subsequence Table:
static std::vector<std::vector<int>>
buildLCS(const std::vector<std::string> &A,
         const std::vector<std::string> &B)
{
    int n = A.size(), m = B.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (A[i - 1] == B[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);

    return dp;
}


// Diff Output:
static void printDiff(const std::vector<std::string> &A,const std::vector<std::string> &B,const std::vector<std::vector<int>> &dp,int i, int j)
{
    if (i > 0 && j > 0 && A[i - 1] == B[j - 1])
    {
        printDiff(A, B, dp, i - 1, j - 1);
        std::cout << "  " << A[i - 1] << "\n";
    }
    else if (j > 0 && (i == 0 || dp[i][j - 1] >= dp[i - 1][j]))
    {
        printDiff(A, B, dp, i, j - 1);
        std::cout << "+ " << B[j - 1] << "\n";
    }
    else if (i > 0)
    {
        printDiff(A, B, dp, i - 1, j);
        std::cout << "- " << A[i - 1] << "\n";
    }
}

void Diff::show(const std::string &A,
                const std::string &B)
{
    auto filesA = buildFileMap(A);
    auto filesB = buildFileMap(B);

    std::unordered_map<std::string, bool> visited;

    for (auto &[path, fileA] : filesA)
    {
        visited[path] = true;

        if (!filesB.count(path))
        {
            std::cout << "Deleted: " << path << "\n";
        }
        else
        {
            fs::path fileB = filesB[path];

            auto oldLines = readLines(fileA);
            auto newLines = readLines(fileB);

            if (oldLines != newLines)
            {
                std::cout << "\n--- " << path << "\n";
                std::cout << "+++ " << path << "\n";

                auto dp = buildLCS(oldLines, newLines);
                printDiff(oldLines, newLines, dp,
                          oldLines.size(), newLines.size());
            }
        }
    }

    for (auto &[path, fileB] : filesB)
    {
        if (!visited[path])
        {
            std::cout << "Added: " << path << "\n";
        }
    }
}
