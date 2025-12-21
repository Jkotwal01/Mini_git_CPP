/*
Commit Design (Very Important):
Commit = Metadata + Snapshot reference.
Each commit has:
    commit_id (unique)
    parent_commit_id (or NONE)
    timestamp
    commit_message

list of tracked files (snapshot)*/
#ifndef COMMIT_H
#define COMMIT_H

#include <string>

class Commit
{
public:
    static void create(const std::string &message);
};

#endif
