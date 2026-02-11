#ifndef DIFF_H
#define DIFF_H

#include <string>

class Diff
{
public:
    static void show(const std::string &commitA,
                     const std::string &commitB);
};

#endif
