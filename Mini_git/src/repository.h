#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>

class Repository
{
public:
    static bool exists(); // Static functions call without creating objetct.
    static void init();
};

#endif
