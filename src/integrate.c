#include "../includes/minishell.h"

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && str_ichr(path, ':') > -1)
	{
		dir = str_ndup(path, str_ichr(path, ':'));
		bin = join_path(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += str_ichr(path, ':') + 1;
	}
	return (cmd);
}

// static int is_empty(char *str) {
//     while (*str) {
//         if (!isspace(*str)) return 0;
//         str++;
//     }
//     return 1;
// }

void shell_loop(t_command *cmd, char **env)
{
    if (!cmd) return;

    // Handle exit command
    if (strcmp(cmd->command, "exit") == 0) {
        printf("Exiting minishell...\n");
        free_commands(cmd);
        exit(0);
    }

    if (strcmp(cmd->command, "cd") == 0) {
        execute_cd(cmd->arguments);
        return ;
    }

    if (strcmp(cmd->command, "echo") == 0)
    {
        execute_echo(cmd->arguments);
        return ;
    }
    // Handle export as a built-in
    if (strcmp(cmd->command, "export") == 0) {
        ft_export(&g_data.env_list, cmd->arguments);
        return;
    }

    // Handle env as a built-in
    if (strcmp(cmd->command, "env") == 0) {
        ft_env(g_data.env_list, cmd->arguments);
        return;
    }
    // Check for pipes in the command
    if (strchr(cmd->full_command, '|')) {
        handle_pipes(cmd, env);
        return;
    }

    // Regular command execution (non-piped)
    char **full_command = ft_split(cmd->full_command, ' ');
    if (!full_command) {
        perror("minishell");
        return;
    }

    // pid_t pid = fork();
    // if (pid < 0) {
    //     perror("fork");
    //     free_split(full_command);
    //     return;
    // }

    // if (pid == 0) {  // Child process
    //     char *path;
    //     if (str_ichr(cmd->command, '/') > -1) {
    //         path = cmd->command;
    //     } else {
    //         path = get_path(cmd->command, env);
    //     }

    //     if (execve(path, full_command, env) == -1) {
    //         perror("minishell");
    //         free_split(full_command);
    //         exit(EXIT_FAILURE);
    //     }
    // } else {  // Parent process
    //     int status;
    //     waitpid(pid, &status, 0);
    //     free_split(full_command);
    // }
    execute(cmd, env); // This will handle built-ins and external commands
}
