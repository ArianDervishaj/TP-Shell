#ifndef _COMMANDS_H_
#define _COMMANDS_H_
#define _POSIX_C_SOURCE 200809L

#include "interface.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

bool is_built_in(cmd_t *cmd);
void exec_builtin(cmd_t *cmd);
void exec_cmd(cmd_t *cmd);
void init_signal_handler();
void handle_sigchld();

#endif