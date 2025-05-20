#include "../includes/minishell.h"

int	str_ichr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

char	*join_path(char *path, char *bin)
{
	char	*joined;
	int		i;
	int		j;

	joined = malloc(sizeof(char) * (str_ichr(path, 0) + str_ichr(bin, 0) + 2));
	i = 0;
	j = 0;
	while (path[j])
		joined[i++] = path[j++];
	joined[i++] = '/';
	j = 0;
	while (bin[j])
		joined[i++] = bin[j++];
	joined[i] = 0;
	return (joined);
}

char	*str_ndup(char *str, unsigned int n)
{
	char			*s;
	unsigned int	i;

	i = 0;
	s = malloc(sizeof(char) * (n + 1));
	while (i < n)
		s[i++] = *str++;
	s[n] = 0;
	return (s);
}

bool    is_blank_line(const char *s)
{
    size_t  i = 0;
    if (!s)
        return (true);
    while (s[i])
    {
        if (!isspace((unsigned char)s[i]))
            return (false);
        i++;
    }
    return (true);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while ((char)c != s[i])
	{
		if (!s[i])
			return (NULL);
		i++;
	}
	return ((char *)&s[i]);
}

int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

t_env *env_from_array(char **env)
{
	t_env *head = NULL;
	t_env *new;
	int i = 0;
	char *eq;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			new = malloc(sizeof(t_env));
			new->name = str_ndup(env[i], eq - env[i]);
			new->value = ft_strdup(eq + 1);
			new->next = head;
			head = new;
		}
		i++;
	}
	return head;
}
