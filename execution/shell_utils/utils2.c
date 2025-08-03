#include "../../includes/minishell.h"

char	*dup_and_check(t_list *current, char **result, int *i)
{
	int	index;

	index = *i;
	result[index] = ft_strdup((char *)current->content);
	ft_gc_add(result[index]);
	if (!result[index])
		return (NULL);
	*i = index;
	return ("Ok");
}

char	**get_args(t_list *args)
{
	char	**result;
	int		i;
	t_list	*current;

	if (!args)
		return (NULL);
	result = ft_malloc(sizeof(char *) * (ft_lstsize(args) + 1));
	if (!result)
		return (NULL);
	i = 0;
	current = args;
	while (current)
	{
		if (!dup_and_check(current, result, &i))
			return (NULL);
		i++;
		current = current->next;
	}
	result[i] = NULL;
	return (result);
}

static int	calculate_total_length(char **args, const char *sep)
{
	int	total_length;
	int	i;

	total_length = 0;
	i = 0;
	while (args[i])
	{
		total_length += ft_strlen(args[i]);
		if (sep)
			total_length += ft_strlen(sep);
		i++;
	}
	return (total_length);
}

char	*ft_strconcat(char **args, const char *sep)
{
	char	*result;
	int		i;
	int		total_length;

	if (!args || !*args)
		return (NULL);
	total_length = calculate_total_length(args, sep);
	if (total_length == 0)
		return (NULL);
	result = ft_malloc(total_length + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (args[i])
	{
		if (i > 0 && sep)
			ft_strlcat(result, sep, total_length + 1);
		ft_strlcat(result, args[i], total_length + 1);
		i++;
	}
	return (result);
}
