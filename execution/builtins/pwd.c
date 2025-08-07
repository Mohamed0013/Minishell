/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:34 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:34 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		if (errno == ENOENT)
			fprintf(stderr, "pwd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file \
or directory\n");
		else if (errno == EACCES)
			fprintf(stderr, "pwd: error retrieving current directory: \
getcwd: Permission denied\n");
		else if (errno == ENAMETOOLONG)
			fprintf(stderr, "pwd: error retrieving current directory: \
getcwd: File name too long\n");
		else
			perror("pwd");
		return (1);
	}
}
