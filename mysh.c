/**
 * Daniyal Adzhiyev
 * CS360: Assignment 2 - Creating Your Own Shell
 * 02/15/2020
 * 
 * This program creates a shell that will allow user's to execute other programs from the shell's command line.
 * 
 * */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

void getCommand();
char **generateCommandStatements();
void changeCurrentDirectory(char*);
void printWorkingDirectory();
void executeExternalCommand(char*, char**);


char command[256]; // variable to hold command statements of the user

/*
 * This function reads the user input from the shell and stores it in a string
 */


// launch point of the shell
int main(int argc, char *argv[])
{
	// string of commands to be read from user
	char *builtInCommands[3] = {"cd", "pwd", "exit"}; // list of built in commands

	// using a do-while loop as this should be executed at least one time
	do
	{

		printf("~$mysh > "); // prints the standard shell output before each command

		getCommand(); // get's the user's command

		char **words = generateCommandStatements(); // holds the array of strings

		// if user's command is to change directory
		if (strcmp(words[0], builtInCommands[0]) == 0)
		{
			changeCurrentDirectory(words[1]);
		}

		// if user's command is to print the directory
		else if (strcmp(words[0], builtInCommands[1]) == 0)
		{
			printWorkingDirectory();
		}

		// if the user doesn't want to exit the shell, than they are attempting to execute an external command
		else if (strcmp(words[0], builtInCommands[2]) != 0)
		{
			executeExternalCommand(words[0], words);
		}
		else {
			exit(EXIT_SUCCESS); // user's command is to exit shell
		}
		free(words); // free's the memory space from the heap
	}
	
	// loop while the user does not request to exit the shell
	while (strcmp(command, builtInCommands[2]) != 0);
	

}

void getCommand()
{
	// if fgets fails to read command print error statement
	if (fgets(command, sizeof(command), stdin) == NULL)
	{
		printf("failed to read command");
	}

	// the command is stored, and remove any possible new line characters in command
	else
	{
		
		int length = strlen(command); // get's the length of characters in the command
									  // if the last character is a newline character
		if (command[length - 1] == '\n')
		{
			command[length - 1] = 0; // remove the newline character from end
		}
	}
}
/*
 * This function splits the user's single string command into an array of string commands
 */
char **generateCommandStatements()
{
	char *delimeter = " "; // use "space character" to split up the words
	char *saveptr; // pointer to the user's commands
	char **words = malloc(50); // allocates memory of the heap for the array of words
	char *header = strtok_r(command, delimeter, &saveptr); // first word in the command
	
	words[0] = header; // sets the first word to the header
	int i; 
	// max number of words to put in the array of strings
	for (i = 1; i < 50; i++)
	{
		words[i] = strtok_r(NULL, delimeter, &saveptr); // adds the rest of the words to the array of strings.
		// if there is no more words break from the loop
		if (words[i] == NULL)
		{
			break;
		}
	}

	// returns the array of strings
	return words;
}

/*
 * This function change's the current directory path
 */
void changeCurrentDirectory(char *path)
{
	int retVal = chdir(path); // change's directory to specified path

	// if directory does not exist or there is an error the return value will not be 0
	if (retVal != 0)
	{
		perror(""); // print error to user
	}
}
/*
 * This function prints the current directory
 */
void printWorkingDirectory()
{
	char cwd[256]; // string to hold the path

	// checks to see there is a current directory
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd); //prints the directory
	}
}

/*
 * This method takes an external command and attempts to execute it
 */
void executeExternalCommand(char *externalCommand, char **words)
{
	int pid = getpid(); // parent pid
	int ret = fork();   // creates a new identitical process

	// if the fork fails
	if (ret < 0)
	{
		printf("Failed to create process\n");
	}
	// in child process
	if (ret == 0)
	{
		execvp(externalCommand, words); // replace this process with the process the user requests
		printf("failed to execute command\n"); // if this executes than the processes wasn't replaced, and there was an error.
		exit(EXIT_FAILURE); // exits the child process so a duplicate process is not created upon failure
	}
	else
	{
		wait(NULL); // in parent, wait for child process to finish before returning to shell
	}
}
