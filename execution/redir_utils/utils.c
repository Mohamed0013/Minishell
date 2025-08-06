#include "../includes/minishell.h"
#define HEREDOC_MSG "minishell: heredoc: EOF reached withoutdelimiter\n"
#define HEREDOC_TMP_FILE "/tmp/heredoc.tmp"

volatile sig_atomic_t	*ft_sigint_track(void)
{
	static volatile sig_atomic_t	signal;

	return (&signal);
}

void	ft_sigint_handler(int sig)
{
	(void)sig;
	*ft_sigint_track() = 1;
}

int	ft_getc(FILE *stream)
{
	char	c;

	c = EOF;
	(void)(stream);
	if (*ft_sigint_track())
		return (EOF);
	if (read(STDIN_FILENO, &c, 1) <= 0)
		return (EOF);
	return (c);
}

char	*handle_heredoc(char *delimiter)
{
	char	*file;
	char	*line;
	char	*tmp;

	file = NULL;
	line = NULL;
	rl_getc_function = ft_getc;
	*ft_sigint_track() = 0;
	signal(SIGINT, ft_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line && !*ft_sigint_track())
			ft_putstr_fd(HEREDOC_MSG, 2);
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		tmp = ft_strjoin3(file, line, "\n");
		free(file);
		file = tmp;
		free(line);
	}
	free(line);
	rl_getc_function = rl_getc;
	signal(SIGINT, SIG_IGN);
	return (file);
}

static int	handle_input_redirect(t_redir *redir_info)
{
	int	fd;

	// dont forget to handle heredoc!!!!
	if (redir_info->type == TOKEN_REDIRECT_IN)
	{
		if (!redir_info->filename)
		{
			ft_putstr_fd("minishell: ambiguous redirect.\n", 2);
			return (1);
		}
		fd = open(redir_info->filename, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir_info->filename);
			return (1);
		}
	}
	else
	{
		fd = open(HEREDOC_TMP_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir_info->filename);
			unlink(HEREDOC_TMP_FILE);
			return (1);
		}
		ft_putstr_fd(redir_info->value, fd);
		close(fd);
		fd = open(HEREDOC_TMP_FILE, O_RDONLY);
		unlink(HEREDOC_TMP_FILE);
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
