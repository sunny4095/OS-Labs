#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

// helper functions
int change_directory(char** tokens) {
	if (tokens[1] == NULL) {
		printf("Error : no arguments are given\n");
		return 0;
	}
	if (tokens[2] != NULL) {
		printf("Error : More than one argument detected\n");
		return 0;
	}
	int return_val = chdir(tokens[1]);
	if (return_val != 0) {
		printf("Error : couldn't change directories\n");
		return 0;
	}
	return 1;
}

void execute_background(char** tokens, int* n, int* background_processes, int **background_pids) {
	if (*background_processes == 64) {
		printf("Error : Cannot add more background processes");
	}
	else {
		tokens[*n-1] = NULL;
		int cpid = fork();
		if (cpid == 0) {
			int x = execvp(tokens[0],tokens);
			printf("Invalid Command\n");
		}
		else {
			printf("background process with PID %d started\n", cpid);
			setpgid(cpid,0);
			int tmp = 0;
			for (int j=0; j<64; j++) {
				if (*background_pids[j] == -1) {
					*background_pids[j] = cpid;
					(*background_processes)++;
					break;
				}
			}
		}
	}
}

void execute_foreground(char** tokens) {
	int cpid = fork();
	if (cpid == 0) {
		signal(SIGINT,SIG_DFL);
		int x = execvp(tokens[0],tokens);
		printf("Invalid Command\n");
	}
	if (cpid > 0) {
		int pid_of_dead_child = wait(NULL);
	}
}

void execute_piped_process(char **tokens1, char **tokens2) {
    int pipefd[2]; // 0 -> read end, 1 -> write end
    pipe(pipefd);

    int cpid1 = fork();
    if (cpid1 == 0) {
        //child process
        close(pipefd[0]); // close read end
        dup2(pipefd[1],STDOUT_FILENO);  // passing stdout to write end 
        close(pipefd[1]); // close write end
        execvp(tokens1[0],tokens1);
        printf("Invalid Command\n");
        exit(1);
    }

    int cpid2 = fork();
    if (cpid2 == 0) {
        //child process
        close(pipefd[1]); // close write end
        dup2(pipefd[0],STDIN_FILENO); //passing stdin to read end
        close(pipefd[0]); // close read end
        execvp(tokens2[0],tokens2);
        printf("Invalid Command\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int background_processes = 0;
	int* background_pids = (int *)malloc(64 * sizeof(int));
	int exit_handler = 0;

	for (int j=0; j<64; j++) {
		background_pids[j]=-1;
	}

	signal(SIGINT,SIG_IGN);

	while(1) {		

		if (exit_handler) break;

		// reaping dead child proccesses before taking in input

		for (int j=0; j<64; j++) {
			if (background_pids[j] != -1) {
				int status;
				int x = waitpid(background_pids[i],&status,WNOHANG);
				if (x!=0) {
					printf("\nreaped background process with PID %d\n", background_pids[i]);
					background_pids[j] = -1;
					background_processes--;
				}
			}
		}

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		int n=0;
		while(tokens[n]!=NULL) {n++;}
		

        // handling input

		if (tokens[0] == NULL) {continue;}
		else if (strcmp(tokens[0],"cd") == 0) {
			// change directory
			int tmp = change_directory(tokens);
		}
		else if (strcmp(tokens[0],"exit") == 0) {
			// exiting from shell
			exit_handler = 1;
		}
		else {
			// handling basic linux commands
            int pipe_index = -1;
            for (int j = 0; j < n; j++) {
                if (strcmp(tokens[j], "|") == 0) {
                    pipe_index = j;
                    break;
                }
            }
            if (pipe_index != -1) {
                // if piped commands
                tokens[pipe_index] = NULL; 
                char **tokens1 = tokens;
                char **tokens2 = &tokens[pipe_index + 1];

                // Execute piped commands
                execute_piped_process(tokens1, tokens2);
            }
            else {
                if (strcmp(tokens[n-1],"&") != 0) {
                    // foreground command execution
                    execute_foreground(tokens);
                }
                else {
                    // background command execution
                    execute_background(tokens,&n,&background_processes,&background_pids);
                }
            }
		}
		

		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}

    if (background_pids != 0) {
		for (int j=0; j<64; j++) {
			if (background_pids[j] != -1) {
				int status;
				waitpid(background_pids[j],&status,0);
			}
		}
	}

	free(background_pids);

	return 0;
}