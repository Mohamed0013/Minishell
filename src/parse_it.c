#include "../includes/minishell.h"

void parse_and_execute(char *input, char **env)
{
    if (!validate_syntax(input))
        return;
    if (strstr(input, ">>"))
    {
        handle_append_redirection(input, env);
        return ;
    }
    else if (strstr(input, ">"))
    {
        handle_output_redirection(input, env);
        return ;
    }
    if (strchr(input, '|')) {
        handle_pipes(input, env);  // Modified function
        return ;
    }
    // Handle single commands
    t_command *cmd = parse_command(input);
    if (cmd) {
        shell_loop(cmd, env);
        free_command(cmd);
    }
}

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