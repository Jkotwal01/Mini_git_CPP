/*
Branch Model (Understand Before Coding)
A branch is just a pointer to a commit.
You already have:
.mygit/branches/main
Content:
<commit_id>
So creating a branch means:
Copy current branch’s HEAD commit into a new file.
That’s it.
On-Disk Structure (Already Compatible)
.mygit/
├── branches/
│   ├── main
│   ├── dev
│   └── feature-x
├── HEAD
└── objects/

HEAD still contains:
ref: refs/branches/main
*/

#ifndef BRANCH_H
#define BRANCH_H

#include <string>

class Branch
{
public:
    static void create(const std::string &name);
    static void list();
};

#endif
