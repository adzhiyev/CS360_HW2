#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char* argv[]) {
	char command[256];
	char close[] = "exit";
	char cd[] = "cd";
	char pwd[] = "pwd";
	char* myargs[2];
	myargs[0] = "./mysh";
	myargs[1] = NULL;
	
do{

	printf("~$mysh > ");
	fgets(command, sizeof(command), stdin);
    int length = strlen(command);
    
        if(command[length - 1] == '\n') {
                command[length - 1] = 0;

        }
	
	char* delimeter = " ";
	char* saveptr;
	char** words = malloc(50);
	char* header = strtok_r(command, delimeter, &saveptr);
	printf("%s\n", header);
	int i;
	words[0] = header;
	for(i = 1; i < 50; i++) {
		words[i] = strtok_r(NULL, delimeter, &saveptr);
		if(words[i] == NULL) {
			break;
		}
	}

	 if(strcmp(command, cd)==0) {

		printf("changing directory\n");
		chdir(words[1]);

		// int return = chdir("    ");
		// if (return !=0) {
		// perror("ERROR: ");
	}

	else if (strcmp(command, pwd)==0) {

		char cwd[256];

		if(getcwd(cwd, sizeof(cwd))!= NULL) {
		printf("%s\n", cwd);
		}
		
			}
	
	else if(strcmp(command, close) != 0) {
		
		int pid = getpid();
		int ret = fork();
		if(ret ==0) {
			execvp(words[0], words);
		printf("failed to execute command\n");
		}
		else {
		wait(0);
		}
	}

}

free(words);
 while(strcmp(command, close) !=0);
}

