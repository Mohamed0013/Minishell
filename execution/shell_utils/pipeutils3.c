/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeutils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:25 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:25 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipes(t_ast *ast)
{
	int		count;
	t_ast	*current;

	count = 0;
	current = ast;
	while (current)
	{
		if (current->next)
			count++;
		current = current->next;
	}
	return (count);
}

void	cleanup_pipes_on_error(int **pipfds, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		close(pipfds[j][0]);
		close(pipfds[j][1]);
		j++;
	}
}

int	check_redirect_types(t_list *redirections, int *has_output, int *has_input)
{
	t_list	*redir;
	t_token	*token;

	*has_output = 0;
	*has_input = 0;
	redir = redirections;
	while (redir)
	{
		token = (t_token *)redir->content;
		if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND)
			*has_output = 1;
		if (token->type == TOKEN_REDIRECT_IN)
			*has_input = 1;
		redir = redir->next;
	}
	return (0);
}

void	fill_pipes(int **pipfds, int nb_pipes)
{
	int	i;

	i = 0;
	while (i < nb_pipes)
	{
		pipfds[i] = ft_malloc(2 * sizeof(int));
		if (!pipfds[i])
		{
			cleanup_pipes_on_error(pipfds, i);
			perror("ft_malloc failed for pipe");
			ft_exit_withclear(EXIT_FAILURE);
		}
		if (pipe(pipfds[i]) == -1)
		{
			cleanup_pipes_on_error(pipfds, i);
			perror("pipe failed");
			ft_exit_withclear(EXIT_FAILURE);
		}
		i++;
	}
}
