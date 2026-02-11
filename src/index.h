//index.h : Staging Interface
#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <vector>

class Index
{
public:
    static void add(const std::vector<std::string> &paths);
    static bool isStaged(const std::string &file);
};

#endif
