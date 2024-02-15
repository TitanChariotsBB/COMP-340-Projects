#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define EXIT_SUCCESS 0

int shell_change_dir(char *dir_path) {
  // use the chdir() system call to change the current directory
  chdir(dir_path);
}


int shell_file_exists(char *file_path) {
  // use the stat() system call to check if a file exists
  // stat takes a char* for the file path and a char pointer for the output buffer
  // we don't need to check the output buffer, so pointing it to null *SHOULD* work
  // if stat finds the file, it should return EXIT_SUCCESS (0)
  // ignore the compiler warning
  return (stat(file_path, NULL) == EXIT_SUCCESS);
}


int shell_find_file(char *file_name, char *file_path, char file_path_size) {
  // traverse the PATH environment variable to find the absolute path of a file/command
}


int shell_execute(char *file_path, char **argv) {
  // execute the file with the command line arguments
  // use the fork() and exec() system call 
}


int main (int argc, char *argv[]) {
	// get user info
	char *username = getenv("USER");
	char *hostname = getenv("HOSTNAME");

  // for testing purposes
	printf("Username: %s\nHostname: %s\n", username, hostname);

  int exit;
  
  //run the shell
  while (!exit) {
    char input[50];
    printf("username@hostname:path/path$ ");
    fgets(input, 50, stdin);

    if (strstr(input, "exit") != NULL) { // checks if strings are equal
      exit = 0; 
      break; 
    } else {
      printf("%s", input);
    }
  }


  /*
	1. display prompt and wait for user input
		// generate some prompt 
		// e.g. username@hostname:~/david$ ping 
	
	2. filter out whitespace command 
	
	3. if command line contains non-whitespace characters
	
	4. parse command line
		if the specified command is “exit”, terminate the program taking care to release any allocated resources.
		if the specified command is “cd”
			change the current working directory to the specified directory path using shell_change_dir()
		if the command is specified using an absolute path (e.g. /usr/bin/ls), exists in the user’s PATH (e.g. ls) or exists in the current folder (e.g. ./hello_world)
			execute the command using shell_execute()
		else
			report an error message
  */
}
