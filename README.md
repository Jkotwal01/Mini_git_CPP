### mygit — Minimal Version Control System in C++
A lightweight Git-inspired version control system implemented in C++, demonstrating core VCS concepts like staging, commits, branches, and history.

Features:
Initialize a repository: mygit init
Stage files and directories: mygit add <file|dir>
View staged files: mygit status
Create snapshot commits: mygit commit "message."
View commit history: mygit log
Create and list branches: mygit branch [name]
CLI help: mygit help

Usage:
./mygit init
./mygit add src/main.cpp
./mygit commit "Initial commit"
./mygit status
./mygit log
./mygit branch dev
./mygit branch
./mygit help


Installation:
Clone the repository:
git clone <repo_url>
cd mygit

Compile:
g++ -std=c++17 src/*.cpp -o mygit

Directory Structure:
.mygit/
├── HEAD                  # Current branch pointer
├── branches/             # Branch pointers (files contain commit IDs)
├── index                 # Staging area
├── objects/              # Commit objects (metadata + files)
└── logs/commits.log      # Commit history log
