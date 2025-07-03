#include "../../includes/parsing.h"

int get_len(char **s)
{
    int i;

    i = 0;
    if (!s || !*s)
        return (0);
    while (s[i])
        i++;
    return (i);
}