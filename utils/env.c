#include "../includes/minishell.h"

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
