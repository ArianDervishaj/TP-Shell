#include "commands.h"

const char *built_in_commands[] = {
    "cd", "exit", "help", "echo",
    NULL // Marqueur de fin de liste
};

bool is_built_in(cmd_t *cmd) {
  if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) {
    return false;
  }

  for (int i = 0; built_in_commands[i] != NULL; i++) {
    if (strcmp(cmd->argv[0], built_in_commands[i]) == 0) {
      return true;
    }
  }

  return false;
}

void exec_builtin(cmd_t *cmd) {

  if (strcmp(cmd->argv[0], "cd") == 0) {
    
    if (cmd->argv[1] == NULL) { // Pas d'argument au cd
      fprintf(stderr, "cd: missing operand\n");

    } else if (chdir(cmd->argv[1]) != 0) {
      perror("cd");
    }
  
  } else if (strcmp(cmd->argv[0], "exit") == 0) {
    exit(0);

  } else if (strcmp(cmd->argv[0], "help") == 0) {
    printf("Available built-in commands: cd, exit, help, echo\n");

  } else if (strcmp(cmd->argv[0], "echo") == 0) {
    for (int i = 1; cmd->argv[i] != NULL; i++) {
      printf("%s ", cmd->argv[i]);
    }
    printf("\n");
  }
}

void exec_cmd(cmd_t *cmd) {
  pid_t pid;
  pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);

  } else if (pid == 0) { // process enfant
    if (execvp(cmd->argv[0], cmd->argv) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
    }

  } else {
    // Processus parent
    if (cmd->foreground) {
      // Attendre la fin du processus enfant
      int status;
      waitpid(pid, &status, 0);

      // Affiche le code de retour si le processus s'est terminé normalement
      if (WIFEXITED(status)) {
        printf("Foreground job exited with code %d\n", WEXITSTATUS(status));
      }
    } else {
      // Ne pas attendre pour un processus en arrière-plan
      printf("Background job started with PID %d\n", pid);
    }
  }
}

void handle_sigchld() {
  while (waitpid(-1, NULL, WNOHANG) > 0) {
    // Attendre la fin des processus enfants sans bloquer
  }
}

void init_signal_handler() {
  struct sigaction sa;
  sa.sa_handler = handle_sigchld;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // Éviter les interruptions inutiles
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
}