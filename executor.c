#include "includes/minishell.h"

/**
 * Executes built-in commands (e.g., echo, cd, export).
 * Returns 0 on success, 1 on failure, or -1 if not a built-in.
 */
int execute_builtin(t_command *cmd) {
    if (!cmd || !cmd->command)
        return -1;

    if (strcmp(cmd->command, "echo") == 0)
        return ft_echo(cmd->arguments);
    else if (strcmp(cmd->command, "cd") == 0)
        return ft_cd(&g_env, cmd->arguments);
    else if (strcmp(cmd->command, "pwd") == 0)
        return ft_pwd();
    else if (strcmp(cmd->command, "export") == 0)
        return ft_export(&g_env, cmd->arguments);
    else if (strcmp(cmd->command, "unset") == 0)
        return ft_unset(&g_env, cmd->arguments);
    else if (strcmp(cmd->command, "env") == 0)
        return ft_env(g_env);
    else if (strcmp(cmd->command, "exit") == 0)
        return ft_exit(cmd->arguments);
    else
        return -1; // Not a built-in
}

/**
 * Executes external commands using fork() + execvp().
 * Handles pipes and redirections if needed.
 */
void execute_external(t_command *cmd, char **env) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(cmd->command, cmd->arguments) == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("minishell: fork");
    } else {
        // Parent process waits for child
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
    }
}

/**
 * Main executor function.
 * Routes commands to built-ins or external programs.
 */
void execute(t_command *cmd, char **env) {
    int builtin_status;

    if (!cmd)
        return;

    builtin_status = execute_builtin(cmd);
    if (builtin_status == -1) {
        // Not a built-in → execute as external program
        execute_external(cmd, env);
    } else {
        // Built-in executed; set exit status
        g_exit_status = builtin_status;
    }
}