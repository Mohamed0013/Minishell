#include "../includes/minishell.h"

static int ft_sep(char c, char sep)
{
    if (c == sep)
        return (1);
    return (0);
}

static int cnt_strings(char const *s, char c)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (s[i])
    {
        while (s[i] && ft_sep(s[i], c))
            i++;
        if (s[i])
            count++;
        while (s[i] && !ft_sep(s[i], c))
            i++;
    }
    return (count);
}

static void    ft_get_strings_length(char **splited, char const *s, char c)
{
    int i;
    int len;

    i = 0;
    while (*s)
    {
        len = 0;
        while (*s && ft_sep(*s, c))
            s++;
        while (*s && !ft_sep(*s, c))
        {
            len++;
            s++;
        }
        splited[i] = malloc(sizeof(char) * len + 1);
        i++;
    }
}

static void    ft_fill_strings(char **splited, char const *s, char c)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    while (s[i])
    {
        k = 0;
        while (s[i] && ft_sep(s[i], c))
            i++;
        while (s[i] && !ft_sep(s[i], c))
        {
            splited[j][k] = s[i];
            i++;
            k++;
        }
        splited[j][k] = '\0';
        j++;
    }
}

char **ft_split(char const *s, char c)
{
    int count;
    char    **splited;

    if (!s || !c)
        return (NULL);
    count = cnt_strings(s, c);
    splited = malloc(sizeof(char *) * count + 1);
    if (splited == NULL)
        return (NULL);
    ft_get_strings_length(splited, s, c);
    ft_fill_strings(splited, s, c);
    return (splited);
}