#include "includes/parsing.h"
#include "includes/minishell2.h"
#include "includes/minishell.h"

t_data g_data;

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void handle_sigquit(int sig)
{
	(void)sig;
	// Ignore SIGQUIT (Ctrl-\)
	write(1, "\b\b	\b\b", 6);
}

void free_env_list(t_env *env)
{
	t_env *tmp;
	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

int main(int ac, char **av, char **env)
{
	char		*input;
	t_ast		*cmd;
	int 		value;

	value = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	(void)av;
	if (ac > 1)
		return (0);

	g_data.env_list = env_from_array(env);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL) // Handle EOF (Ctrl+D)
			break;
		if (*input == '\0') // Handle empty input
		{
			free(input);
			continue;
		}
		add_history(input);
		if (is_blank_line(input))
		{
			free(input);
			continue;
		}
		cmd = parser(input);
		free(input);
		if (cmd == NULL)
		{
			free(input);
			continue;
		}
		// print_argv(cmd);
		// free_commands(cmd);	 // Free the command structure
		value = shell_execute(cmd, env, value);
		if (value == 2) // Handle special return value
			break ;
	}
	free_commands(cmd);
	free_env_list(g_data.env_list); // Free the environment list
	if (value != 2)
		return value;
}