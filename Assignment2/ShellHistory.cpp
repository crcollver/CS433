// =================================
// CS 433 Programming assignment 2
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/9/2019
// Course: CS433 (Operating Systems)
// Description : Basic shell program with history feature using fork() and execvp()
// =================================

#include "ShellHistory.h"
#include <iostream>
#include <cstring>

using namespace std;

// Default Constructor for ShellHistory
ShellHistory::ShellHistory()
{
    commands = 0;
    removed = 0;
    max = 10;
}

// Constructor for user specified MAX commands
ShellHistory::ShellHistory(unsigned int max)
{
    commands = 0;
    removed = 0;
    this->max = max;
}

// destructor
ShellHistory::~ShellHistory() {}

// PURPOSE: automatically removes a command when list grows past 10 (instructions prog2.pdf)
void ShellHistory::remove_last_command()
{
    history.erase(history.begin()); // remove beginning element from vector
    removed++;
}

// PURPOSE: adds a command to the history
// PARAM: command to add to history - even if command is unable to execute
void ShellHistory::add_command(char* line)
{
    char *toadd = new char; // create new char pointer to copy contents of line
    strcpy(toadd, line); // copy contents of line to new address at toadd
    history.push_back(toadd); // add the copied value to our data structure
    commands++;
    // if the size grows to be larger than max number of history commands allowed
    if(history.size() > max) 
        remove_last_command();
}

// PURPOSE: returns most recent command to be run again by shell
char* ShellHistory::get_most_recent()
{
    if(history.empty())
        throw Underflow(); // No commands in history
    else
        return history.back(); // return the most recent command
}

// PURPOSE: gets a command given its identifier in the list
// PARAM: the identifier the user gives, which will be converted to list index
char* ShellHistory::get_at_index(int index)
{
    // calculates a 0-9 index based on how many commands have been removed
    // since user can enter !n, n > 10
    int count = (index - removed) - 1;
    if(history.empty())
        throw Underflow(); // No elements in history
    else if((unsigned int)count > max || count < 0 || (unsigned int)count >= history.size())
       throw OutofRange(); // Command identifier out of range
    else
        return history.at(count); // return command at index
}

// PURPOSE: prints 10 most recent commands (most to least recent)
void ShellHistory::print_history()
{
    if(history.empty())
        throw Underflow(); // No elements in history
    else
    {
        int num = commands; // loop for length of vector
        for(int i = (history.size()) - 1; i >= 0; i--)
        {
             cout << num << " " << history[i] << endl;
             num--;
        }
           
    }
}