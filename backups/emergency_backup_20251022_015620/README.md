# MiniGit - A Simple Version Control System

MiniGit is a lightweight, command-line version control system written in C++ that provides essential Git-like functionality for managing code repositories. It supports basic version control operations including repository initialization, file staging, committing, branching, merging, and more.

## Features

- **Repository Management**: Initialize and manage Git-like repositories
- **File Staging**: Add files to staging area before committing
- **Commit System**: Create commits with messages and track file changes
- **Branch Management**: Create, list, and switch between branches
- **Merge Operations**: Merge branches with conflict detection
- **Commit History**: View detailed commit logs and history
- **Status Tracking**: Check repository status and staged files
- **Checkout**: Switch between commits and branches

## Prerequisites

### Required Software
- **C++ Compiler**: GCC 7.0+ with C++17 support
- **MSYS2/MinGW**: For Windows users (recommended)
- **OpenSSL**: For cryptographic hash functions
- **Make**: For building the project

### Windows Setup
1. Install [MSYS2](https://www.msys2.org/)
2. Open MSYS2 terminal and install required packages:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-openssl
   pacman -S make
   ```

### Linux Setup
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libssl-dev

# CentOS/RHEL
sudo yum install gcc-c++ openssl-devel make
```

## Building the Project

### Method 1: Using the Batch Script (Windows)
```cmd
compile.bat
```

### Method 2: Using the Shell Script (Unix/Linux)
```bash
chmod +x build.sh
./build.sh
```

### Method 3: Using Make
```bash
make
```

### Method 4: Manual Compilation
```bash
# Create object directory
mkdir -p obj

# Compile source files
g++ -std=c++17 -Wall -Wextra -Iinclude -I. -c src/*.cpp -o obj/

# Link the executable
g++ obj/*.o -o mini_git.exe -lssl -lcrypto -lstdc++fs
```

## Usage

### Basic Commands

#### Initialize a Repository
```bash
./mini_git.exe init
```
Creates a new MiniGit repository in the current directory.

#### Add Files to Staging
```bash
./mini_git.exe add <filename>
./mini_git.exe add file1.txt file2.cpp
```
Stages files for the next commit.

#### Commit Changes
```bash
./mini_git.exe commit -m "Your commit message"
```
Creates a new commit with staged files.

#### View Commit History
```bash
./mini_git.exe log
```
Displays the commit history with details.

#### Check Repository Status
```bash
./mini_git.exe status
```
Shows the current repository status and staged files.

### Branch Management

#### Create a New Branch
```bash
./mini_git.exe branch <branch-name>
```

#### List All Branches
```bash
./mini_git.exe branch
```

#### Switch to a Branch or Commit
```bash
./mini_git.exe checkout <branch-name>
./mini_git.exe checkout <commit-hash>
```

#### Merge Branches
```bash
./mini_git.exe merge <branch-name>
```

### Getting Help
```bash
./mini_git.exe help
```
Displays all available commands and their usage.

## Project Structure

```
MiniGit/
├── include/                 # Header files
│   ├── branch.hpp          # Branch management
│   ├── commit.hpp          # Commit system
│   ├── file_manager.hpp    # File operations
│   ├── logger.hpp          # Logging utilities
│   ├── merge.hpp           # Merge operations
│   ├── repo.hpp            # Main repository class
│   └── utils.hpp           # Utility functions
├── src/                    # Source files
│   ├── branch.cpp
│   ├── commit.cpp
│   ├── file_manager.cpp
│   ├── logger.cpp
│   ├── main.cpp            # Main entry point
│   ├── merge.cpp
│   ├── repo.cpp
│   └── utils.cpp
├── obj/                    # Object files (generated)
├── build.sh               # Unix build script
├── compile.bat            # Windows build script
├── makefile               # Make configuration
└── README.md              # This file
```

## Repository Structure

When you initialize a MiniGit repository, it creates the following structure:

```
.git/
├── objects/               # Stored file contents
├── refs/                 # Branch references
│   └── heads/           # Branch heads
├── index                 # Staging area
└── HEAD                  # Current branch/commit reference
```

## Examples

### Basic Workflow
```bash
# Initialize repository
./mini_git.exe init

# Create some files
echo "Hello World" > hello.txt
echo "int main() { return 0; }" > main.cpp

# Add files to staging
./mini_git.exe add hello.txt main.cpp

# Check status
./mini_git.exe status

# Commit changes
./mini_git.exe commit -m "Initial commit"

# View history
./mini_git.exe log
```

### Branching Workflow
```bash
# Create and switch to new branch
./mini_git.exe branch feature-branch
./mini_git.exe checkout feature-branch

# Make changes and commit
echo "New feature" > feature.txt
./mini_git.exe add feature.txt
./mini_git.exe commit -m "Add new feature"

# Switch back to main
./mini_git.exe checkout main

# Merge the feature branch
./mini_git.exe merge feature-branch
```

## Technical Details

### Hash Algorithm
- Uses SHA-256 for file content hashing
- Commit IDs are generated using SHA-256 of commit data

### File Storage
- Files are stored in `.git/objects/` directory
- Content-addressed storage using file hashes
- Staging area tracks files ready for commit

### Branch Management
- Branches are stored as references in `.git/refs/heads/`
- HEAD file tracks current branch or commit
- Supports fast-forward and merge commits

### Conflict Resolution
- Automatic conflict detection during merges
- Conflict files are marked with conflict markers
- Manual resolution required for conflicts

## Troubleshooting

### Common Issues

#### Build Errors
- **OpenSSL not found**: Install OpenSSL development libraries
- **C++17 not supported**: Update your compiler to GCC 7.0+
- **Permission denied**: Ensure you have write permissions in the directory

#### Runtime Errors
- **"Not a MiniGit repository"**: Run `mini_git.exe init` first
- **"File not found"**: Check if the file exists and path is correct
- **"Branch not found"**: Verify the branch name exists with `mini_git.exe branch`

### Debug Mode
For debugging, you can examine the `.git` directory structure:
```bash
# View repository structure
ls -la .git/
ls -la .git/objects/
cat .git/HEAD
cat .git/index
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. Please check the license file for details.

## Version History

- **v1.0.0**: Initial release with basic Git functionality
- Core features: init, add, commit, log, checkout, branch, merge, status

## Future Enhancements

- Remote repository support
- Pull/Push operations
- Advanced merge strategies
- Interactive staging
- Cherry-picking
- Rebase functionality
- GUI interface

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the help command: `./mini_git.exe help`
3. Examine the repository structure
4. Create an issue in the project repository

---

**Note**: This is a simplified version control system for educational purposes. For production use, consider using the full Git implementation.
