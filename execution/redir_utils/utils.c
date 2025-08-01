#include "../includes/minishell.h"

static int	handle_input_redirect(t_redir *redir_info)
{
	int	fd;

	fd = open(redir_info->filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir_info->filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
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

	if (redir_info->type == 3)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir_info->filename, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir_info->filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
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
		if (redir_info->type == 1)
		{
			if (handle_input_redirect(redir_info) != 0)
				return (1);
		}
		else if (redir_info->type == 2 || redir_info->type == 3)
		{
			if (handle_output_redirect(redir_info) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
