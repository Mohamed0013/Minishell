#include "../includes/minishell.h"

void handle_append_redirection(char *command, char **env) {
    char **parts = ft_split(command, '>');
    if (!parts || !parts[1]) {
        fprintf(stderr, "minishell: syntax error near unexpected token `>>`\n");
        free_split(parts);
        return;
    }

    char *cmd = parts[0];
    char *file = parts[1];

    // Trim whitespace
    cmd = ft_strtrim(cmd, " ");
    file = ft_strtrim(file, " ");

    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("minishell");
        free_split(parts);
        return;
    }

    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    t_command *parsed_cmd = parse_command(cmd);
    shell_loop(parsed_cmd, env);
    free_command(parsed_cmd);

    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    free_split(parts);
}