# Mini Git - Version Control System

An enterprise-grade version control system implemented in C++ that replicates core Git functionality. This project demonstrates advanced software engineering principles including distributed version control architecture, staging mechanisms, commit management, branch operations, and comprehensive file tracking with diff algorithms.

## 🚀 Core Features

- **Repository Initialization** - Initialize version-controlled repositories with complete metadata structure
- **Intelligent File Staging** - Stage individual files or entire directory trees with recursive traversal
- **Atomic Commits** - Create immutable snapshots with metadata tracking and parent-child relationships
- **Branch Management** - Full branch lifecycle management including creation, listing, and pointer manipulation
- **Branch Switching** - Context-aware branch checkout with automatic workspace restoration and conflict prevention
- **Commit History** - Comprehensive commit log with chronological tracking and branch association
- **Status Tracking** - Real-time staging area inspection and file state monitoring
- **Advanced Diff Engine** - Line-by-line comparison using optimized Longest Common Subsequence (LCS) algorithm

## 📁 Project Structure

```
Mini_git/
├── src/
│   ├── main.cpp           # Entry point and command dispatcher
│   ├── repository.cpp/h   # Repository initialization
│   ├── index.cpp/h        # Staging area management
│   ├── commit.cpp/h       # Commit creation and management
│   ├── branch.cpp/h       # Branch operations
│   ├── checkout.cpp/h     # Branch switching logic
│   ├── log.cpp/h          # Commit history display
│   ├── status.cpp/h       # Staging status display
│   ├── diff.cpp/h         # Diff algorithm (LCS-based)
│   └── help.cpp/h         # Help command
├── .mygit/                # Repository metadata (created after init)
│   ├── objects/           # Commit snapshots
│   ├── branches/          # Branch pointers
│   ├── logs/              # Commit logs
│   ├── HEAD               # Current branch reference
│   └── index              # Staging area
└── mygit.exe              # Compiled executable
```

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
```bash
mygit init
```
Creates a `.mygit` directory with the necessary structure for version control.

### Add Files to Staging Area
```bash
# Add a single file
mygit add file.txt

# Add multiple files
mygit add file1.txt file2.txt

# Add a directory (recursively)
mygit add src/
```

### Check Status
```bash
mygit status
```
Displays all files currently in the staging area.

### Create a Commit
```bash
mygit commit "Your commit message"
```
Creates a snapshot of all staged files with the provided message.

### View Commit History
```bash
mygit log
```
Shows all commits with their IDs, branches, and messages.

### Branch Management

**List all branches:**
```bash
mygit branch
```
Shows all branches with an asterisk (*) marking the current branch.

**Create a new branch:**
```bash
mygit branch feature-branch
```
Creates a new branch pointing to the current commit.

### Switch Branches
```bash
mygit checkout feature-branch
```
Switches to the specified branch and restores the workspace to that branch's state.

**Note:** You must commit or clear staged changes before switching branches.

### Compare Commits
```bash
mygit diff <commitA> <commitB>
```
Shows the differences between two commits using the Longest Common Subsequence (LCS) algorithm.

### Get Help
```bash
mygit help
```
Displays available commands and usage examples.

## 🔍 Architecture & Implementation

### Repository Structure
The system implements a hierarchical metadata architecture:
- `.mygit/objects/` - Object database storing immutable commit snapshots with file trees
- `.mygit/branches/` - Branch reference storage maintaining commit pointers
- `.mygit/logs/` - Persistent commit history with chronological ordering
- `.mygit/HEAD` - Symbolic reference to the current working branch
- `.mygit/index` - Staging area index tracking file state transitions

### Commit Workflow
1. **Staging Phase** - Files are indexed in the staging area (`.mygit/index`) with path normalization
2. **ID Generation** - Unique commit identifiers are generated using timestamp-based hashing
3. **Snapshot Creation** - Complete file tree is serialized to `.mygit/objects/<commitID>/files/`
4. **Metadata Persistence** - Commit metadata including parent references, branch associations, and messages are stored
5. **Reference Update** - Branch pointers are atomically updated to maintain commit graph integrity
6. **Index Reset** - Staging area is cleared to prepare for subsequent operations

### Branch Switching Algorithm
1. **Validation** - Verifies target branch existence in the reference database
2. **State Verification** - Prevents data loss by checking for uncommitted staged changes
3. **Workspace Cleanup** - Removes files tracked by the current commit snapshot
4. **State Restoration** - Reconstructs workspace from target branch's commit tree
5. **Reference Update** - Updates HEAD symbolic reference to new branch pointer

### Diff Algorithm Implementation
Implements the **Longest Common Subsequence (LCS)** dynamic programming algorithm for optimal line-by-line difference computation:
- **Common Lines** - Unchanged content displayed without modification markers
- **Deletions** - Lines exclusive to source commit marked with `-` prefix
- **Additions** - Lines exclusive to target commit marked with `+` prefix
- **Complexity** - O(n×m) time and space complexity for n and m line counts

## 💡 Example Workflow

```bash
# Initialize repository
mygit init

# Create and add files
echo "Hello World" > hello.txt
mygit add hello.txt

# Commit changes
mygit commit "Initial commit"

# Create a new branch
mygit branch feature

# Make changes and commit
echo "New feature" > feature.txt
mygit add feature.txt
mygit commit "Add feature"

# View history
mygit log

# Switch back to main
mygit checkout main

# Compare commits
mygit diff <commit1> <commit2>
```

## ⚙️ Technical Specifications

### Technology Stack
- **C++17 Standard** - Leveraging modern C++ features including structured bindings, filesystem library, and STL containers
- **std::filesystem** - Cross-platform file system abstraction for portable I/O operations
- **Dynamic Programming** - Optimized LCS algorithm implementation for efficient diff computation
- **STL Containers** - Utilizing unordered_map, vector, and set for high-performance data structures

### Software Engineering Principles
- **Modular Architecture** - Feature-based namespace organization with clear separation of concerns
- **Encapsulation** - Static helper functions maintaining implementation privacy
- **Cross-Platform Compatibility** - Path normalization ensuring consistent behavior across operating systems
- **RAII Pattern** - Resource management through automatic file handle cleanup

### Limitations
- Commit IDs are timestamp-based (not cryptographic hashes like Git)
- No merge functionality
- No remote repository support
- No conflict resolution
- Limited to text file diffing

## 🤝 Future Enhancements

Potential extensions for production-grade deployment:
- **Merge Strategies** - Implement three-way merge with conflict resolution
- **Remote Repository Support** - Add network protocol for distributed collaboration
- **Advanced Diff Algorithms** - Myers' diff, patience diff, or histogram diff implementations
- **Comprehensive Testing** - Unit tests, integration tests, and performance benchmarks
- **Enhanced Error Handling** - Robust exception handling and transaction rollback mechanisms
- **Cryptographic Hashing** - SHA-256 based commit IDs for collision resistance
- **Compression** - Object database compression for storage optimization

## 📝 License

This project is open source and available for educational purposes.

## 🎓 Technical Competencies Demonstrated

This implementation showcases expertise in:
- **Distributed Version Control Architecture** - Complete Git-like internals implementation
- **File System Engineering** - Advanced file operations, directory traversal, and metadata management
- **Algorithm Design** - Dynamic programming (LCS), graph structures (commit DAG), and optimization techniques
- **Systems Programming** - Command-line interface design, process management, and I/O handling
- **Software Architecture** - Modular design, namespace organization, and scalable code structure

## 🐛 Known Issues

- Checkout may fail if files have been manually deleted
- Large binary files are not handled efficiently
- No support for .gitignore-style file exclusion

## 📧 Support

For questions or issues, please refer to the source code comments or the help command:
```bash
mygit help
```

---

**Built with C++ | Inspired by Git**
