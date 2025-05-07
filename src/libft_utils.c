#include "../includes/minishell.h"

void    free_split(char **split)
{
    int i;

    i = 0;
    if (!split)
        return ;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}