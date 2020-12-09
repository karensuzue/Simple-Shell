#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  char *input;
  char *token;
  char **argv;
  size_t readchar;

  argv = malloc(65 * sizeof(char *));

  do {
    fprintf(stderr, "> ");

    size_t bytesize = 10;
    input = malloc(bytesize * sizeof(char *));
    readchar = getline(&input, &bytesize, stdin);

    if (readchar == -1) {
      exit(EXIT_FAILURE);
    }

    char *newline_ptr = strchr(input, '\n');
    *newline_ptr = '\0';

    token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
      argv[i] = token;
      token = strtok(NULL, " ");
      i++;
    }


    pid_t pid = fork();

    if (pid == 0) {
      if (execvp(argv[0], argv) == -1) {
        fprintf(stderr, "Command not found \n");
        return 1;
      }
    }

    else {
      int status;
      waitpid(pid, &status, 0);
      fprintf(stderr, "Command exited with status %d\n", WEXITSTATUS(status));
    }


  } while (readchar != -1);

  return 0;
}
