#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "interface.h"

#define PROMPT "gsh> " //Prompt which will be displayed when waiting for user inputs
#define  DELIMIERS " \t" //params of commands can be seperated by the following chars

//Parse a user command into its arguments
//IN:
//  user_input: a string containing the full command. IT WILL BE MODIFIED BY THE FUNCTION.
//OUT:
//  cmd: command updated from user input string (i.e. command input)
//  return: -1 in case of error otherwise 0.
int parse_command(char *user_input, cmd_t *cmd) {

  //Initialize a simple command (empty, simple, foreground)
  cmd->argv = NULL;
  cmd->argc = -1;
  cmd->argv2 = NULL;
  cmd->argc2 = -1,
  cmd->type = CMD_SIMPLE;
  cmd->foreground = true;

  //Separate string in different token (i.e. command name + params + &)
  do {
    //A new element will be added
    cmd->argc += 1;

    //Allocate a new pointer on char for next argv element
    if((cmd->argv = realloc(cmd->argv, (cmd->argc+1)*sizeof(char*))) == NULL)
      die_errno("parse_command::realloc");

    //Get the adress of the next token (could be NULL to indicate end of argv)
    cmd->argv[cmd->argc] = strtok(user_input, DELIMIERS);
    user_input = NULL; //Useless to execute it each time but easier than having two different strtok calls
  } while (cmd->argv[cmd->argc] != NULL); // while there are still tokens

  //Search for redirection symbols (>, >>, <) in the first cmd
  for(int i=0; i < cmd->argc; i++) {
    if(strcmp(">", cmd->argv[i]) == 0)
      cmd->type = CMD_FILEOUT;
    if(strcmp(">>", cmd->argv[i]) == 0)
      cmd->type = CMD_FILEAPPEND;
    if(strcmp("<", cmd->argv[i]) == 0)
      cmd->type = CMD_FILEIN;

    // if symbol was found at i
    if(cmd->type == CMD_FILEOUT || cmd->type == CMD_FILEAPPEND || cmd->type == CMD_FILEIN) {
      if(cmd->argc > i+1) {
        //Allocate a size two table of char* to store file + NULL
        if((cmd->argv2 = realloc(cmd->argv2, 2 * sizeof(char*))) == NULL)
          die_errno("parse_command::realloc");

        // copy argument following symbol to argv2
        cmd->argv2[0] = cmd->argv[i+1];
        cmd->argv2[1] = NULL;
        cmd->argc2 = 1;

        // update argv to ignore symbol on the following args
        cmd->argv[i] = NULL;
        cmd->argc = i;
      }
      else
        return -1; //Nothing after symbol
    }
  }

  //Search for pipe symbols in the first cmd
  // TODO: this look very similar to indirection symbols search -> combine / creat common function ?
  for(int i=0; i < cmd->argc; i++) {
    if(strcmp("|", cmd->argv[i]) == 0) {
      if(cmd->argc > i+1) {  // If no args after | ignore it
        cmd->type = CMD_PIPE;

        //Allocate a new pointer on char for the number of elements after the |
        cmd->argc2 = cmd->argc - i - 1;
        if((cmd->argv2 = realloc(cmd->argv2, (cmd->argc - i) * sizeof(char*))) == NULL)  //one more than argc cause last is NULL
          die_errno("parse_command::realloc");

        //copy pointers to arguments after the | to argv2
        for(int j=0; j < cmd->argc2; j++)
          // copy argument following symbol to argv2
          cmd->argv2[j] = cmd->argv[i+j+1];

        //Add a NULL pointeur at the end of argvs
        cmd->argv2[cmd->argc2] = NULL;

        // update argv to ignore symbol on the following args
        cmd->argv[i] = NULL;
        cmd->argc = i;

        // Only first pipe symbol is considered no more should be found
        break;
      }
    }
  }

  //Check if the last command is a '&' to decide if we should run command
  //as a foreground or background job
  if(strcmp(cmd->argv[cmd->argc-1], "&") == 0) {
    cmd->foreground = false;
    //Delete the & from the command for execution
    cmd->argv[cmd->argc-1] = NULL;
    cmd->argc--;
  }

  return 0;
}

//Check is a string is composed only of a set of characters
//OUT:
//  str:    the string to check (does it contains some chars ?)
//  of:     a string containing the set of characters to verify
//  return: true is the string str is made only of chars in of, false otherwise
static bool str_made_only_of(const char *str, const char *of) {
  while(*str)
    if(strchr(of, *str++) == NULL)
      return false;
  return true;
}

//Wait for the user input and return it
//OUT:
//  user_input: a string with the user input. The user should take care of the
//              allocation.
//  return: 0 if an empty input was given, -1 if the function failed to read stdin
int ask_user_input(char* user_input) {
  // Show the prompt and get user input
  printf(PROMPT);
  if(fgets(user_input, ARG_MAX, stdin) == NULL)
    return -1;

  //Ensure that the end of line chars are removed
  user_input[strcspn(user_input, "\r\n")] = 0;

  // if string is only made of delimiters -> replace by empty string
  if(str_made_only_of(user_input, DELIMIERS))
    user_input[0] = '\0';

  return user_input[0];
}

//Dispose of the command, which free some allocated memory
//IN:
//  cmd: the command to dispose of
void dispose_command(cmd_t *cmd) {
  free(cmd->argv);
  free(cmd->argv2);
}

//Print command for debugging
void print_command(cmd_t cmd) {
  printf("Command:\n");

  printf("argc: %d\n", cmd.argc);

  if(cmd.argv == NULL)
    printf("argv: not allocated\n");
  else {
    printf("argv (print up to NULL element): ");
    int i=0;
    while(cmd.argv[i] != NULL)
      printf("%s ", cmd.argv[i++]);
    printf("\n");
  }

  printf("argc2: %d\n", cmd.argc2);

  if(cmd.argv2 == NULL)
    printf("argv2: not allocated\n");
  else {
    printf("argv2 (print up to NULL element): ");
    int i = 0;
    while(cmd.argv2[i] != NULL)
      printf("%s ", cmd.argv2[i++]);
    printf("\n");
  }

  printf("type: %d (", cmd.type);
  switch (cmd.type)
  {
  case CMD_SIMPLE:
    printf("CMD_SIMPLE");
    break;
  case CMD_PIPE:
    printf("CMD_PIPE");
    break;
  case CMD_FILEOUT:
    printf("CMD_FILEOUT");
    break;
  case CMD_FILEIN:
    printf("CMD_FILEIN");
    break;
  case CMD_FILEAPPEND:
    printf("CMD_FILEAPPEND");
    break;

  default:
    break;
  }
  printf(")\n");

  printf("foreground: ");
  printf(cmd.foreground ? "true\n" : "false\n");
}
