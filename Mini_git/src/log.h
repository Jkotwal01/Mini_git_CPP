/*
Conceptual Flow (Understand First)
When user runs:
./mygit log
Your program should:
Read .mygit/HEAD
Identify current branch
Read branch file → latest commit ID
Loop:
Load commit’s meta
Print commit info
Move to parent
Stop at NONE
This is linked-list traversal, not recursion magic.
// (Newest → Oldest)
*/
#ifndef LOG_H
#define LOG_H

class Log
{
public:
    static void show();
};

#endif
