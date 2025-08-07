#include "../expansion.h"

bool	prev_not_heredoc(t_token *token)
{
	if (!token || !token->prev)
		return (true);
	return (token->prev->type != TOKEN_HEREDOC);
}

bool	prev_not_redirect(t_token *token)
{
	if (!token || !token->prev)
		return (true);
	return (!ft_token_is_redirection(token->prev->type));
}

bool	is_redirection_filename(t_token *token)
{
	if (!token || !token->prev)
		return (false);
	return (ft_token_is_redirection(token->prev->type));
}

int		check_ambiguous_redirect(t_token *token, char *expanded)
{
	if (!is_redirection_filename(token))
		return (0);
	
	// Check if the original token value contained quotes
	// If it did, then it shouldn't be treated as ambiguous
	if (token->value && (ft_strchr(token->value, '"') || ft_strchr(token->value, '\'')))
		return (0);
	
	// Check if the original token contains a variable (starts with $)
	// and the expanded result is empty (undefined variable)
	if (token->value && token->value[0] == '$' && (!expanded || !*expanded))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (1);
	}
	
	// Split the expanded string into words to count them
	char **words = ft_split(expanded, ' ');
	if (!words)
		return (0);
	
	// Count the number of words
	int word_count = 0;
	while (words[word_count])
		word_count++;
	
	// If more than one word, it's ambiguous
	if (word_count > 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(expanded, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		free_split(words);
		return (1);
	}
	
	free_split(words);
	return (0);
}
