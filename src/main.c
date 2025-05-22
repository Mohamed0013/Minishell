#include "../includes/minishell.h"

void  free_command(t_command *cmd) {
    if (cmd) {
        free(cmd->full_command);
        free(cmd->command);
        if (cmd->arguments) {
            for (int i = 0; cmd->arguments[i]; i++) {
                free(cmd->arguments[i]);
            }
            free(cmd->arguments);
        }
        free(cmd);
    }
}

void parse_and_execute(char *input, char **env) {
    // Check for redirection operators first
    if (strstr(input, ">>")) {
        handle_append_redirection(input, env);
        return;
    } else if (strstr(input, ">")) {
        handle_output_redirection(input, env);
        return;
    }

    // Split the input by '|'
    char **pipeline_commands = ft_split(input, '|');
    if (!pipeline_commands) {
        perror("minishell");
        return;
    }

    for (int i = 0; pipeline_commands[i]; i++) {
        char *command = ft_strtrim(pipeline_commands[i], " \t\n");
        if (!command || *command == '\0') {
            fprintf(stderr, "minishell: syntax error near unexpected token `|`\n");
            free(command);
            free_split(pipeline_commands);
            return;
        }

        // Parse and execute the command
        t_command *cmd = parse_command(command);
        if (!cmd) {
            fprintf(stderr, "minishell: failed to parse command\n");
            free(command);
            free_split(pipeline_commands);
            return;
        }

        shell_loop(cmd, env);
        free_command(cmd);
        free(command);
    }

    free_split(pipeline_commands);
}

int main(int ac, char **av, char **env) {
    char *input;
    (void)ac;
    (void)av;
    if (!env) {
        fprintf(stderr, "No environment variables found.\n");
        return 1;
    }
    while (1) {
        input = readline("minishell> ");
        if (!input) {
            printf("\n");
            break;
        }

        if (*input) {
            add_history(input);
            parse_and_execute(input, env);
        }

        free(input);
    }

    return 0;
}