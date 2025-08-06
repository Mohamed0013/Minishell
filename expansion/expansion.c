#include "../includes/minishell.h"
#include "expansion.h"

static void	do_word_splitting(t_token *token, char *expanded)
{
	char	**words;
	t_token	*new_token;
	t_token	*next_backup;
	int		i;

	words = ft_split(expanded, ' ');
	if (!words || !words[0])
	{
		if (words)
			free_split(words);
		free(token->value);
		token->value = expanded;
		return ;
	}

	next_backup = token->next;
	
	// Replace current token with first word
	free(token->value);
	token->value = ft_strdup(words[0]);
	
	// Add remaining words as new tokens
	i = 1;
	while (words[i])
	{
		new_token = ft_malloc(sizeof(t_token));
		if (!new_token)
			break ;
		new_token->value = ft_strdup(words[i]);
		new_token->type = TOKEN_WORD;
		new_token->is_quoted = 0;
		new_token->next = next_backup;
		new_token->prev = token;
		
		if (next_backup)
			next_backup->prev = new_token;
		token->next = new_token;
		token = new_token;
		i++;
	}
	
	free_split(words);
	free(expanded);
}

static int	is_export_assignment(t_token *token)
{
	t_token	*first_token;

	if (!token)
		return (0);
	// Find the first token in the command
	first_token = token;
	while (first_token->prev)
		first_token = first_token->prev;
	
	// Check if first token is "export" and current token has '='
	if (first_token && first_token->value 
		&& ft_strcmp(first_token->value, "export") == 0
		&& token->value && ft_strchr(token->value, '='))
		return (1);
	return (0);
}

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
				if (ft_strchr(expanded, ' ') && !current->is_quoted 
					&& !is_export_assignment(current))
				{
					do_word_splitting(current, expanded);
				}
				else
				{
					free(current->value);
					current->value = expanded;
				}
			}
		}
		current = current->next;
	}
}
