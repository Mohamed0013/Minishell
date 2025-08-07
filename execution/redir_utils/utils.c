/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:36 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:36 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_ft(t_redir *redir_info)
{
	int	fd;

	if (!redir_info->filename)
	{
		ft_putstr_fd("minishell: ambiguous redirect.\n", 2);
		return (-1);
	}
	fd = open(redir_info->filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir_info->filename);
		return (-1);
	}
	return (fd);
}

static int	ft_ft2(t_redir *redir_info)
{
	int	fd;

	fd = open(HEREDOC_TMP_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir_info->filename);
		unlink(HEREDOC_TMP_FILE);
		return (-1);
	}
	ft_putstr_fd(redir_info->value, fd);
	close(fd);
	fd = open(HEREDOC_TMP_FILE, O_RDONLY);
	unlink(HEREDOC_TMP_FILE);
	return (fd);
}

static int	handle_input_redirect(t_redir *redir_info)
{
	int	fd;

	if (redir_info->type == TOKEN_REDIRECT_IN)
		fd = ft_ft(redir_info);
	else
		fd = ft_ft2(redir_info);
	if (fd < 0 || dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redirect(t_redir *redir_info)
{
	int	flags;
	int	fd;

	if (redir_info->type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir_info->filename, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir_info->filename);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_list *redirections)
{
	t_list	*redir;
	t_redir	*redir_info;

	redir = redirections;
	while (redir)
	{
		redir_info = (t_redir *)redir->content;
		if (redir_info->type == TOKEN_HEREDOC
			|| redir_info->type == TOKEN_REDIRECT_IN)
		{
			if (handle_input_redirect(redir_info) != 0)
				return (1);
		}
		else if (redir_info->type == TOKEN_APPEND
			|| redir_info->type == TOKEN_REDIRECT_OUT)
		{
			if (handle_output_redirect(redir_info) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
