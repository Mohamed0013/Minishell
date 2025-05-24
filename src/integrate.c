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

int is_builtin(t_command *cmd) {
    if (!cmd || !cmd->command) return 0;

    const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++) {
        if (strcmp(cmd->command, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void execute_builtin(t_command *cmd) {
    if (strcmp(cmd->command, "echo") == 0) {
        execute_echo(cmd->arguments);
    } else if (strcmp(cmd->command, "cd") == 0) {
        execute_cd(cmd->arguments);
    } else if (strcmp(cmd->command, "pwd") == 0) {
        // Implement pwd
    } else if (strcmp(cmd->command, "export") == 0) {
        // Implement export
    } else if (strcmp(cmd->command, "unset") == 0) {
        // Implement unset
    } else if (strcmp(cmd->command, "env") == 0) {
        // Implement env
    } else if (strcmp(cmd->command, "exit") == 0) {
        // Implement exit
    }
}

void execute_single_command(t_command *cmd, char **env) {
    if (!cmd || !cmd->command) return;

    char *path = get_path(cmd->command, env);
    if (execve(path, cmd->arguments, env) == -1) {
        perror("execve");
        free(path);
        exit(EXIT_FAILURE);
    }
    free(path);
}

void shell_loop(t_command *cmd, char **env) {
    if (!cmd) return;

    if (is_builtin(cmd)) {
        execute_builtin(cmd);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execute_single_command(cmd, env);
        exit(EXIT_SUCCESS);
    } else {
        waitpid(pid, NULL, 0);
    }
}
