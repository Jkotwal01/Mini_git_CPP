#include "help.h"
#include <iostream>

void Help::show()
{
    std::cout << "mygit - A simple version control system\n\n";

    std::cout << "Usage:\n";
    std::cout << "  mygit <command> [options]\n\n";

    std::cout << "Commands:\n";
    std::cout << "  init                    Initialize a new repository\n";
    std::cout << "  add <file|dir>          Add files to staging area\n";
    std::cout << "  status                  Show staged files\n";
    std::cout << "  commit \"msg\"            Create a commit\n";
    std::cout << "  log                     Show commit history\n";
    std::cout << "  branch                  List branches\n";
    std::cout << "  branch <name>           Create a new branch\n";
    std::cout << "  checkout <name>         switching between branches\n";
    std::cout << "  diff <c1> <c2>          displays the differences between two input data sets\n";
    std::cout << "  help                    Show this help message\n\n";

    std::cout << "Example:\n";
    std::cout << "  mygit init\n";
    std::cout << "  mygit add src/main.cpp\n";
    std::cout << "  mygit commit \"initial commit\"\n";
}
