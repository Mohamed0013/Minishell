#include "../../includes/minishell.h"

int	ft_env(t_env *env_list, char **args)
{
	t_env	*current;

	if (!env_list)
		return (0);
	current = env_list;
	if (args && args[1] != NULL)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	while (current)
	{
		if (current->name && current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
	return (0);
}
