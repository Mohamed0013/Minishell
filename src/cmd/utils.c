#include "../includes/minishell.h"

int ft_split_size(char **split) {
    int size = 0;
    while (split[size])
        size++;
    return size;
}

int get_len(char **s)
{
    if (!s || !*s)
        return (0);
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}
