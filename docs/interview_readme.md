# Mini Git - Under the Hood (Interview Guide)

This document provides a deep dive into the internal workings, algorithms, core logic, and source code structure of the Mini Git Version Control System. It is designed to explain *how* the system works behind the scenes.

## 🧠 Core Logic & Complete Working

Mini Git operates on a simplified but faithful representation of a distributed version control system's internals. 

### 1. Repository Structure (`.mygit/`)
When initialized, a `.mygit` directory is created at the root of the project. It contains:
- **`HEAD`**: A text file containing the symbolic reference to the current active branch (e.g., `ref: refs/heads/main`).
- **`index`**: A flat text file acting as the staging area. It simply lists the relative paths of files that are currently staged for the next commit.
- **`branches/`**: A directory where each file represents a branch name. The content of the file is the ID of the commit that the branch currently points to.
- **`objects/`**: The object database. Each commit is a directory named by its timestamp-based Commit ID.
- **`logs/`**: Contains `commits.log` which appends a chronological record of all commits made across all branches.

### 2. The Commit Workflow (How a Snapshot is Saved)
1. **Staging (`mygit add`)**: File paths are written to the `.mygit/index` file. If a directory is added, it is recursively traversed, and all its file paths are added to the index.
2. **Committing (`mygit commit`)**:
   - A unique Commit ID is generated using the current Unix timestamp.
   - The current branch is read from `HEAD`, and its latest commit (parent) is retrieved.
   - A new directory is created in `objects/<commitID>/`.
   - The files listed in the `index` are physically copied into `objects/<commitID>/files/` maintaining their directory structure.
   - A `meta` file is written inside the commit directory containing the commit ID, parent ID, branch name, and commit message.
   - The branch pointer in `branches/<branch_name>` is updated to this new Commit ID.
   - The action is appended to `logs/commits.log`.
   - The `.mygit/index` is cleared.

### 3. Branching and Checkout
- **Branching**: Creating a branch simply creates a new text file in the `branches/` directory containing the current commit ID.
- **Checkout**: When switching branches, the system reads the target branch's latest commit ID. It then clears the current workspace files (that are tracked) and restores the files by copying them from `objects/<targetCommitID>/files/` back into the workspace. The `HEAD` file is updated to point to the new branch.

## 🧮 Detailed Algorithms

### Diff Algorithm: Longest Common Subsequence (LCS)
To compute the difference between two commits, Mini Git uses the classic **Longest Common Subsequence (LCS)** dynamic programming algorithm. 

**Algorithm Steps:**
1. **File Mapping**: For both commit A and commit B, a map of relative file paths to their physical locations in the object database is built.
2. **File Tracking**: The system checks for files added or deleted by comparing the keys of these two maps.
3. **Line-by-Line Diff**: For files present in both commits that have modified content, the files are read line-by-line into arrays (or vectors) of strings.
4. **LCS Table Construction**: A 2D DP table is constructed where `dp[i][j]` represents the length of the longest common subsequence of the first `i` lines of file A and the first `j` lines of file B.
   ```cpp
   if (A[i - 1] == B[j - 1])
       dp[i][j] = dp[i - 1][j - 1] + 1;
   else
       dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
   ```
5. **Backtracking (Generating the Diff Output)**: The DP table is traversed backwards from `dp[n][m]`. 
   - If lines match, it's a common line (unchanged).
   - If `dp[i][j-1] >= dp[i-1][j]`, it implies a line was added in B (marked with `+`).
   - Otherwise, a line was deleted from A (marked with `-`).
   - This guarantees an optimal O(N*M) time and space complexity for diffing a file of N lines and M lines.

## 📂 Source Code Details (`src/` Directory)

Here is a detailed breakdown of every file in the `src` directory and its purpose:

- **`main.cpp`**
  - **Purpose**: The entry point of the application. 
  - **Logic**: Acts as a command dispatcher. It parses command-line arguments (`argc`, `argv`), validates input, and routes the execution to the appropriate module (e.g., calling `Repository::init()` for `mygit init`).

- **`repository.cpp` & `repository.h`**
  - **Purpose**: Handles repository initialization.
  - **Logic**: Implements the `init()` function which creates the `.mygit` directory and its subdirectories (`objects`, `branches`, `logs`). It also sets up the initial `HEAD` file pointing to the default branch and creates an empty `index` file.

- **`index.cpp` & `index.h`**
  - **Purpose**: Manages the staging area.
  - **Logic**: Implements the `add()` functionality. It uses `std::filesystem::recursive_directory_iterator` to traverse directories and resolve file paths. The paths are appended to the `.mygit/index` text file.

- **`commit.cpp` & `commit.h`**
  - **Purpose**: Core engine for creating immutable snapshots (commits).
  - **Logic**: Reads the staged files from `index`, generates a timestamp-based ID, copies the files to the `.mygit/objects` folder, writes metadata, updates branch pointers, and flushes the staging area.

- **`branch.cpp` & `branch.h`**
  - **Purpose**: Manages branch lifecycle.
  - **Logic**: Implements listing existing branches by iterating through the `.mygit/branches/` directory, and creating new branches by creating a new file in that directory populated with the current `HEAD` commit ID.

- **`checkout.cpp` & `checkout.h`**
  - **Purpose**: Handles branch switching and workspace restoration.
  - **Logic**: Changes the `HEAD` reference. Replaces the files in the user's working directory with the exact copies stored in the target branch's latest commit object inside `.mygit/objects/`.

- **`log.cpp` & `log.h`**
  - **Purpose**: Displays commit history.
  - **Logic**: Sequentially reads and prints the `.mygit/logs/commits.log` file, which contains the chronological history of all commits.

- **`status.cpp` & `status.h`**
  - **Purpose**: Displays the current status of the staging area.
  - **Logic**: Reads the `.mygit/index` file and lists the files that are currently staged and ready to be committed.

- **`diff.cpp` & `diff.h`**
  - **Purpose**: Computes and displays the differences between two commits.
  - **Logic**: Implements the Longest Common Subsequence (LCS) algorithm. It builds file maps for both commits, detects added/deleted files, and performs line-by-line comparison using a 2D dynamic programming array for modified files.

- **`help.cpp` & `help.h`**
  - **Purpose**: Provides user assistance.
  - **Logic**: Simply prints the usage guide, available commands, and syntax instructions to the standard output.
