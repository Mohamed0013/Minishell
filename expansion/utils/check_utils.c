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
