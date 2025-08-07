/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:31 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:31 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check3(char *current_dir, char **new_dir)
{
	char	*last_slash;

	*new_dir = getcwd(NULL, 0);
	if (!*new_dir)
	{
		printf("minishell: cd: error retrieving current directory\
: getcwd: cannot access parent directories: No such \
file or directory\n");
		last_slash = strrchr(current_dir, '/');
		if (last_slash)
		{
			*last_slash = '\0';
			setenv("PWD", current_dir, 1);
		}
		return (1);
	}
	return (0);
}

static int	check2(void)
{
	if (chdir("..") == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	check(char **current_dir, char **new_dir)
{
	struct stat	parent_stat;

	if (stat("..", &parent_stat) == -1)
	{
		printf("minishell: cd: error retrieving parent directory:\
		%s\n", strerror(errno));
		return (1);
	}
	*current_dir = getcwd(NULL, 0);
	if (!*current_dir)
	{
		printf("minishell: cd: error retrieving current directory\
: getcwd: cannot access parent directories: No such\
file or directory\n");
		return (1);
	}
	if (check2() == 1)
	{
		free(*current_dir);
		return (1);
	}
	if (check3(*current_dir, new_dir) == 1)
	{
		free(*current_dir);
		return (1);
	}
	setenv("PWD", *new_dir, 1);
	return (0);
}

static int	f_check(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) == -1)
		perror("minishell: cd");
	return (0);
}

void	execute_cd(char **arguments)
{
	char	*current_dir;
	char	*new_dir;

	current_dir = NULL;
	new_dir = NULL;
	if (!arguments || !arguments[1])
	{
		f_check();
		return ;
	}
	if (strcmp(arguments[1], "..") == 0)
	{
		if (check(&current_dir, &new_dir) == 0)
		{
			free(current_dir);
			free(new_dir);
		}
		return ;
	}
	if (chdir(arguments[1]) == -1)
		fprintf(stderr, "minishell: cd: %s: %s\n", arguments[1],
			strerror(errno));
}
