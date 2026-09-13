# Mini Git - Version Control System

A lightweight, local version control system built in C++ that replicates the core command-line experience of Git.

## 🛠️ Building the Project

### Prerequisites
- C++17 or later
- A C++ compiler (GCC, Clang, or MSVC)
- Standard library with `<filesystem>` support

### Compilation

**Using g++:**
```bash
g++ -std=c++17 src/*.cpp -o mygit
```

**Using MSVC (Visual Studio):**
```bash
cl /EHsc /std:c++17 src/*.cpp /Fe:mygit.exe
```

## 📖 Usage Guide

### Initialize a Repository
Creates a `.mygit` directory with the necessary structure for version control.
```bash
mygit init
```

### Add Files to Staging Area
Add files to the staging area to prepare them for a commit.
```bash
# Add a single file
mygit add file.txt

# Add multiple files
mygit add file1.txt file2.txt

# Add a directory (recursively)
mygit add src/
```

### Check Status
Displays all files currently in the staging area.
```bash
mygit status
```

### Create a Commit
Creates a snapshot of all staged files with the provided message.
```bash
mygit commit "Your commit message"
```

### View Commit History
Shows all commits with their IDs, branches, and messages.
```bash
mygit log
```

### Branch Management
Shows all branches or creates a new branch.
```bash
# List all branches
mygit branch

# Create a new branch
mygit branch feature-branch
```

### Switch Branches
Switches to the specified branch and restores the workspace to that branch's state. 
*Note: You must commit or clear staged changes before switching branches.*
```bash
mygit checkout feature-branch
```

### Compare Commits
Shows the differences between two commits.
```bash
mygit diff <commitA> <commitB>
```

### Get Help
Displays available commands and usage examples.
```bash
mygit help
```

---
*For a detailed look under the hood (architecture, core logic, and algorithms), please read the [Interview Guide](docs/interview_readme.md).*
