#include "commands.h"
#include "common.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1024

int main() {
  init_signal_handler();

  // Buffer for user input
  char user_input[INPUT_BUFFER_SIZE];

  while (1) {
    // Prompt and wait for user input
    if (ask_user_input(user_input) == -1) {
      perror("Error reading input");
      continue;
    }

    // Exit the shell if the user types "exit"
    if (strcmp(user_input, "exit") == 0) {
      printf("Exiting shell...\n");
      break;
    }

    // Skip empty input
    if (strlen(user_input) == 0) {
      continue;
    }

    // Parse the command
    cmd_t *cmd = malloc(sizeof(cmd_t));
    if (cmd == NULL) {
      perror("malloc");
      continue;
    }

    if (parse_command(user_input, cmd) == -1) {
      fprintf(stderr, "Failed to parse command\n");
      free(cmd);
      continue;
    }

    // Debugging: print the parsed command
    print_command(*cmd);

    // Execute the command
    if (is_built_in(cmd)) {
      exec_builtin(cmd);
    } else {
      exec_cmd(cmd);
    }

    // Dispose of the command
    dispose_command(cmd);
    free(cmd);
  }

  return EXIT_SUCCESS;
}
