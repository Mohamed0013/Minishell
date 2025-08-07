/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeutils4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:26 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:26 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	save_restore_fds(t_list *redirections, int *saved_fds)
{
	if (redirections)
	{
		saved_fds[0] = dup(STDIN_FILENO);
		saved_fds[1] = dup(STDOUT_FILENO);
		if (handle_redirections(redirections) != 0)
		{
			if (saved_fds[0] != -1)
				close(saved_fds[0]);
			if (saved_fds[1] != -1)
				close(saved_fds[1]);
			return (1);
		}
	}
	return (0);
}

void	restore_fds(t_list *redirections, int *saved_fds)
{
	if (redirections)
	{
		if (saved_fds[0] != -1)
		{
			dup2(saved_fds[0], STDIN_FILENO);
			close(saved_fds[0]);
		}
		if (saved_fds[1] != -1)
		{
			dup2(saved_fds[1], STDOUT_FILENO);
			close(saved_fds[1]);
		}
	}
}
