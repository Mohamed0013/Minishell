#include "includes/minishell.h"

int	free_input(char *input)
{
	free(input);
	return (1);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(1, "\b\b	\b\b", 6);
	g_data.exit_status = sig + 128;
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_data.exit_status = sig + 128;
}

void	initial_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
