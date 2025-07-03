#include "../includes/parsing.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// void	free_split(char **split)
// {
// 	int i;

// 	if (!split)
// 		return;
// 	i = 0;
// 	while (split[i])
// 	{
// 		free(split[i]);
// 		i++;
// 	}
// 	free(split);
// }

void	free_commands(t_ast *cmd)
{
	t_ast *current;
	t_list *args;
	t_list *redirs;

	if (!cmd)
		return;
	current = cmd;
	while (current)
	{
		args = current->args;
		while (args)
		{
			free(args->content);
			args = args->next;
		}
		redirs = current->redirections;
		while (redirs)
		{
			t_token *token = (t_token *)redirs->content;
			free(token->value);
			free(token);
			redirs = redirs->next;
		}
		t_ast *next = current->next;
		free(current);
		current = next;
	}
}

int	str_ichr(const char *str, char c)
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
