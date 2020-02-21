#pragma once

// =================================
// CS 433 Programming assignment 2
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/9/2019
// Course: CS433 (Operating Systems)
// Description : Basic shell program with history feature using fork() and execvp()
// =================================

#include <vector>

/**
 * ShellHistory is the data structure to hold all commands entered by the user
 * in the shell.  Should be able to retrieve most recent command executed
 * and keep access to the 10 most recent commands.  Command numbers can grow
 * beyond 10, so if there are 35 commands, the 10 most recent are 26 to 35.
 */

class ShellHistory 
{
private:
    std::vector <char*> history; // a vector of commands (strings)
    int commands; // stores what numbered command we are on for this session
    int removed; // how many commands have been removed automatically
    unsigned int max; // max number of commands; DEFAULT = 10

    // PURPOSE: automatically removes a command when list grows past 10 (instructions prog2.pdf)
    void remove_last_command();
public:
    // PURPOSE: adds a command to the history
    // PARAM: command to add to history - even if command is unable to execute
    void add_command(char* line);

    // PURPOSE: gets most recent command to run again
    char* get_most_recent();

    // PURPOSE: gets a command given its identifier in the list
    // PARAM: the identifier the user gives, which will be converted to queue index
    char* get_at_index(int index);

    // PURPOSE: prints most recent commands (most to least recent)
    void print_history();

    // classes for error handling in driver
    class Underflow{};
    class OutofRange{};

    ShellHistory();
    ShellHistory(unsigned int max);
    ~ShellHistory();
};