#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#define EXIT_SUCCESS 0

int shell_change_dir(char *dir_path) {
  // use the chdir() system call to change the current directory
  // returns 0 on success and -1 if error occurs
  return chdir(dir_path);
}


int shell_file_exists(char *file_path) {
  // use the stat() system call to check if a file exists
  // stat takes a char* for the file path and a char pointer for the output buffer
  // we don't need to check the output buffer, so pointing it to null *SHOULD* work
  // if stat finds the file, it should return true (1)
  struct stat buffer[1000];
  return (stat(file_path, buffer) == EXIT_SUCCESS);
}

// TODO: test this function
int shell_find_file(char *file_name, char *file_path, char file_path_size) {
  // traverse the PATH environment variable to find the absolute path of a file/command
  const char* path = getenv("PATH");
  char *sep = (char*) malloc(file_path_size * sizeof(char));
  sep = strsep(*path, file_name);
  if(sep == NULL){
    free(sep);
    return -1;
  }
  else{
    file_path = strdup(sep);
    free(sep);
    return 0;
  }
}


int shell_execute(char *file_path, char **argv) {
  // execute the file with the command line arguments
  // use the fork() and exec() system call 
  pid_t pid = fork();
  if (pid < 0) {
    printf("Fork failed!\n");
    return -1;
  } else if (pid == 0) {
    execv(file_path, argv);
    exit(0);
  }
  else {
    wait(NULL);
    return EXIT_SUCCESS;
  }
}


int main (int argc, char *argv[]) {
	// get user info
	char *username = getenv("USER");
	char *hostname = getenv("HOSTNAME");
  char wd[100];

  // for testing purposes
	printf("Username: %s\nHostname: %s\n", username, hostname);

  int exit = 0;
  
  //run the shell
  while (!exit) {
    // find current directory
    getcwd(wd, sizeof(wd)); // TODO: replace /home/$USERNAME with "~"

    // get user input
    char input[50];
    printf("%s@hostname:%s$ ", username, wd);
    fgets(input, sizeof(input), stdin);

    // split user input into tokens
    const char *delim = " \n";
    char *token;
    char *tokens[10]; // array of token strings
    int idx = 0;
    token = strtok(input, delim);
    tokens[idx] = token;
    while (token != NULL) {
      idx++;
      token = strtok(NULL, delim);
      tokens[idx] = token;
    }

    // parse command
    char *command = tokens[0];
    if (!strcmp(command, "exit") && tokens[1] == NULL) { // checks if strings are equal
      exit = 0;
    } else if (!strcmp(command, "cd")) {
      // ------------------- CD COMMAND -------------------------
      if (tokens[1] != NULL) {
        char *path = tokens[1];
        if (shell_change_dir(path) < 0) {
          printf("Error: path not found\n");
        }
      } else {
        // throw error
        printf("Error: path not specified\n");
      }
      // --------------------------------------------------------
    } else {
      // absolute file path
      if (shell_file_exists(command)) {
        if (shell_execute(command, tokens) < 0) {
          printf("Error: could not execute file\n");
        }

      // starts with ./
      } else if (strstr(command, "./") == command) { 
        char *toRemove = "./";
        char *new_cmd = strtok(command, delim);
        if (shell_execute(new_cmd, tokens) < 0) {
          printf("Error: could not execute file\n");
        }
      
      // look for executable in PATH directories
      } else {
        // TODO: call shell_find_file() correctly
        printf("Error: could not find executable: %s\n", strerror(errno));
      }
    }
  }
}
