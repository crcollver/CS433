// =================================
// CS 433 Programming assignment 2
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/9/2019
// Course: CS433 (Operating Systems)
// Description : Basic shell program with history feature using fork() and execvp()
// =================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> // both types and wait should be included according to man pages
#include <sys/wait.h>
#include <cstring>
#include <cctype>
#include <iostream>
#include "ShellHistory.h"

#define MAX_LINE 80 // Maximum length of a command
using namespace std;

// PURPOSE: checks if command is a background process, returns true if so
// PARAM: the command to check
bool is_background_process(char line[]);

// PURPOSE: splits given command into tokens to place into args array which is returned
// PARAMS: command to split into tokens, array to store the arguments
char* split_command(char line [], char* args[]);

// PURPOSE: forks a child process and executes a command
// PARAM: list of arguments to execute, whether process should be ran in background
void execute_command(char* args[], bool background);

// PURPOSE: prints 'osh>' prompt, gets user input, cuts off newline,
//      and returns filled line array.
// PARAM: the pointer to the line to be filled
char* get_command(char line[]);

int main(int argc, char* argv[])
{
    char line[MAX_LINE]; // entire command
    char* args[MAX_LINE/2 + 1]; // all tokens from command
    ShellHistory history; // data structure to handle history of shell
    int should_run = 1;
    bool background = false; // is set if user indicates background process '&'
    bool is_history = false; // is set if user types 'history'
    bool digit = true; // sets false when user types !n command that has chars after '!'
    cout << "CS 433 Programming assignment 2" << endl;
	cout << "Author: Cameron Collver and Nick Dujakovich" << endl;
	cout << "Date: 10/9/2019" << endl;
	cout << "Course: CS433 (Operating Systems)" << endl;
	cout << "Description : Basic Shell program with history feature using fork() and execvp()" << std::endl;
	cout << "=================================" << endl;
    while(should_run)
    {
        line[0] = *get_command(line); // get a command from user

        // try/catch block for various history feature commands
        try{
            // if user types 'history'
            if(strcmp(line, "history") == 0)
            {
                history.print_history(); // only print history
                is_history = true; // set history flag
            }

            // if user types '!!' for most recent command
            else if(strcmp(line, "!!") == 0)
                strcpy(line, history.get_most_recent()); // fetch most recent command place in line

            // if user wants command at 'n' index and types '!n'
            else if(line[0] == '!')
            {
                line[0] = '0'; // set the '!' to an int
                unsigned int i = 1; // start at the presumed digit
                // for entire command line
                while(i < strlen(line))
                {
                    // if char is not an int
                    if(!isdigit(line[i]))
                    {
                         digit = false; // set digit flag to false
                         break;
                    }     
                    i++;
                }
                // if every char passed as an int
                if(digit)
                {
                    int h_index = atoi(line); // convert the string to int 
                    strcpy(line, history.get_at_index(h_index)); // get command at the index store in line
                }
                // else we have a command that is not recognized
                else
                    throw ShellHistory::OutofRange();
            }
        }
        catch(ShellHistory::OutofRange)
        { 
            fprintf(stderr, "No such command in history.\n");
            continue;
        }
        catch(ShellHistory::Underflow)
        { 
            fprintf(stderr, "No commands in history.\n");
            continue;
        }

        // if command is supposed to be in background '&', set flag
        if(is_background_process(line))
            background = true;

        // if command is to exit, set flag to 0
        if(strcmp(line, "exit") == 0)
            should_run = 0;

        // else command is cleared to be executed
        else if(!is_history && digit)
        {
            history.add_command(line); // add the command to history
            args[0] = split_command(line, args); // split command into tokens and place in args array
            execute_command(args, background); // execute commands using fork and execvp
        }
        is_history = false; // reset history flag to default
        digit = true; // reset digit flag to default
        background = false; // reset background flag to default
    }
    return 0;
}

// PURPOSE: prints 'osh>' prompt, gets user input, cuts off newline,
//      and returns filled line array.
// PARAM: the pointer to the line to be filled
char* get_command(char line[])
{
    printf("osh> ");
    fflush(stdout); // prints prompt without needing /n"
    fgets(line, MAX_LINE, stdin); // gets command from user
    line[strcspn(line, "\n")] = 0; // removes new line when user presses enter (stackoverflow)
    return line;
}

// PURPOSE: checks if command is a background process, returns true if so
// PARAM: the command to check
bool is_background_process(char line[])
{
    int i = strlen(line) - 1;
    // if the last char is '&' then it is background process
    if(strcmp(&line[i], "&") == 0)
        return true;
    // otherwise it is not
    else
        return false;
}

// PURPOSE: splits given command into tokens to place into args array which is returned
// PARAMS: command to split into tokens, array to store the arguments
char* split_command(char line[], char* args[])
{
    char* token; // used to store each parsed token
    token = strtok(line, " "); // create a token up to the space in the line
    int i = 0;
    // while token doesn't contain a NULL value (end of string)
    while (token != NULL)
    {
        args[i] = token; // place token into args array
        token = strtok(NULL, " "); // get the next argument of command
        i++;
    }
    // if last character is '&' then remove it and replace with NULL
    if(strcmp(args[i - 1], "&") == 0)
        args[i - 1] = NULL;
    // else just place NULL terminator after end of argument list
    else
        args[i] = NULL; // man pages and instructions say args list must be terminated by null pointer
    return *args;
}

// PURPOSE: forks a child process and executes a command
// PARAM: list of arguments to execute, whether process should be ran in background
void execute_command(char* args[], bool background)
{
    int pid = fork(); // create the fork
    // if fork fails
    if(pid < 0)
        fprintf(stderr, "Fork failed.");
            
    // child process
    else if(pid == 0)
    {
        execvp(args[0], args); // execvp only returns if there is an error
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(1); // exit with error from child process
    }
    // parent process
    else
    {
        // if background flag is false
        if(!background)
            wait(NULL); // make parent process wait for child
        // else make parent and child run concurrently
    }
     /**
     * After reading user input, the steps are:
     * (1) fork a child process using fork()
     * (2) the child process will invoke execvp()
     * (3) parent will invoke wait() unless command included &
     */
}