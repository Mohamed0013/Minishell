#include "../expansion.h"

void	ft_token_init(t_token *new_token, t_token *current, char *value,
		int type)
{
	(void)type;
	new_token->value = ft_strdup(value);
	new_token->type = TOKEN_WORD;
	new_token->is_quoted = 0;
	new_token->next = current->next;
	new_token->prev = current;
	if (current->next)
		current->next->prev = new_token;
	current->next = new_token;
}

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
		return (free_split(words));
	free(token->value);
	token->value = ft_strdup(words[0]);
	current = token;
	i = 1;
	while (words[i])
	{
		new_token = ft_malloc(sizeof(t_token));
		ft_token_init(new_token, current, words[i], TOKEN_WORD);
		current = new_token;
		i++;
	}
	free_split(words);
}
