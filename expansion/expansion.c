#include "../includes/minishell.h"
#include "expansion.h"

void	expand(t_env *env, t_token *token)
{
	t_token	*current;
	char	*expanded;

	if (!env || !token)
		return ;
	current = token;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_arg(current->value, env, current);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
