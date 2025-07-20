#include "includes/parsing.h"
#include "includes/minishell2.h"
#include "includes/minishell.h"

t_data	g_data;

void	free_env_list(t_env *env)
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

void	minishell(int *value, char **env, char *input, t_ast *cmd)
{
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		if (*input == '\0' && free_input(input))
			continue ;
		add_history(input);
		if (is_blank_line(input) && free_input(input))
			continue ;
		cmd = parser(input);
		if (cmd == NULL && free_input(input))
			continue;
		if (shell_execute(cmd, env, value) == 2)
			break ;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_ast	*cmd;
	int 	value;

	value = 0;
	(void)av;
	if (ac > 1)
		return (0);
	initial_signals();
	g_data.env_list = env_from_array(env);
	minishell(&value, env, input, cmd);
	// return (free_env_and_input(input, value));
}
