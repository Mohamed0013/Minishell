#include "includes/minishell.h"

t_data	g_data;

void	free_env_list(t_env *env)
{
	t_env	*tmp;

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
		initial_signals();
		input = readline("minishell> ");
		signal(SIGINT, SIG_IGN);
		if (input == NULL)
			break ;
		if (*input == '\0' && free_input(input))
			continue ;
		add_history(input);
		if (is_blank_line(input) && free_input(input))
			continue ;
		cmd = parser(input);
		if (cmd)
			*value = shell_execute(cmd, env, *value);
		free(input);
		ft_gc_clear();
		if (*value == 2)
			return ;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_ast	*cmd;
	int		value;

	input = NULL;
	cmd = NULL;
	value = 0;
	(void)av;
	if (ac > 1)
		return (0);
	g_data.env_list = env_from_array(env);
	minishell(&value, env, input, cmd);
	clear_history();
	ft_exit_withclear(value);
}
