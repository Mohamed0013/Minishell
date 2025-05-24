#include "../includes/minishell.h"

char **split_commands(char *input)
{
    char **commands;
    char *token;
    int i;

    i = 0;
    commands = malloc(sizeof(char *) * (strlen(input) + 1));
    if (!commands) return NULL;

    token = strtok(input, "|");
    while (token != NULL) {
        commands[i++] = strdup(token);
        token = strtok(NULL, "|");
    }
    commands[i] = NULL;
    return (commands);
}
