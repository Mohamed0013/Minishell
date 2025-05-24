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

int main(int ac, char **av, char **env) {
    char *input;
    (void)ac;
    (void)av;
    if (!env) {
        write(2, "No environment variables found.\n", 31);
        return 1;
    }
    while (1) {
        input = readline("minishell> ");
        if (!input)
        {
            printf("\n");
            break ;
        }
        if (*input)
        {
            add_history(input);
            parse_and_execute(input, env);
        }
        free(input);
    }

    return 0;
}