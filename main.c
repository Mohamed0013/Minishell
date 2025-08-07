/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:43 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:43 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	minishell(char **env, char *input, t_ast *cmd)
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
			g_data.exit_status = shell_execute(cmd, env, g_data.exit_status);
		free(input);
		ft_gc_clear();
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_ast	*cmd;

	input = NULL;
	cmd = NULL;
	(void)av;
	if (ac > 1)
		return (0);
	g_data.env_list = env_from_array(env);
	minishell(env, input, cmd);
	clear_history();
	ft_exit_withclear(g_data.exit_status);
}
