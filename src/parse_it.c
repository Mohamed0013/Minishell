#include "../includes/minishell.h"

t_command *parse_command(char *command) {
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) {
        perror("malloc");
        return NULL;
    }

    char **args = ft_split(command, ' ');
    if (!args) {
        free(cmd);
        perror("ft_split");
        return NULL;
    }

    cmd->command = ft_strdup(args[0]);
    cmd->arguments = args;
    cmd->full_command = ft_strdup(command);

    return cmd;
}