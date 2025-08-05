#include "../expansion.h"

void	split_expanded_token(t_token *token, const char *expanded_value)
{
	char	**words;
	t_token	*new_token;
	t_token	*current;
	int		i;

	if (!expanded_value || !ft_strchr(expanded_value, ' '))
		return ;
	
	words = ft_split(expanded_value, ' ');
	if (!words || !words[0])
	{
		if (words)
			free_split(words);
		return ;
	}
	
	// Replace current token value with first word
	free(token->value);
	token->value = ft_strdup(words[0]);
	
	// Insert remaining words as new tokens after current token
	current = token;
	i = 1;
	while (words[i])
	{
		new_token = ft_malloc(sizeof(t_token));
		if (!new_token)
			break ;
		new_token->value = ft_strdup(words[i]);
		new_token->type = TOKEN_WORD;
		new_token->is_quoted = 0;
		
		// Insert after current
		new_token->next = current->next;
		new_token->prev = current;
		if (current->next)
			current->next->prev = new_token;
		current->next = new_token;
		
		current = new_token;
		i++;
	}
	
	free_split(words);
}
