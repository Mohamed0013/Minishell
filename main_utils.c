#include "includes/minishell.h"

int free_env_and_input(char *input, int value)
{
	free(input);
	free_env_list(g_data.env_list);
	return (value);
}

int	free_input(char *input)
{
	free(input);
	return (1);
}

void handle_sigquit(int sig)
{
	(void)sig;
	// Ignore SIGQUIT (Ctrl-\)
	write(1, "\b\b	\b\b", 6);
}

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void initial_signals(void)
{
	signal(SIGINT, handle_sigint); // Handle Ctrl+C
	signal(SIGQUIT, handle_sigquit); // Ignore Ctrl+
}