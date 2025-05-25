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
