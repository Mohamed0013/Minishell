#include "../includes/minishell.h"

void execute_echo(char **arguments)
{
    int (i) = 1, (newline) = 1;
    if (!arguments || !arguments[0]) {
        printf("\n");
        return;
    }
    if (arguments[i] && strcmp(arguments[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (arguments[i]) {
        printf("%s", arguments[i]);
        if (arguments[i + 1]) {
            printf(" ");
        }
        i++;
    }

    if (newline) {
        printf("\n"); // Print the newline if not suppressed
    }
}