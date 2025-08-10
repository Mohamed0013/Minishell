/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohdahma <mohdahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:56:21 by mohdahma          #+#    #+#             */
/*   Updated: 2025/08/10 14:56:22 by mohdahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	print_pwd_error(const char *msg)
{
	char *prefix = "pwd: error retrieving current directory: getcwd: ";
	char *tmp;
	char *full_msg;

	tmp = ft_strjoin(prefix, msg);
	if (!tmp)
		return (1);
	full_msg = ft_strjoin(tmp, "\n");
	free(tmp);
	if (!full_msg)
		return (1);
	write(2, full_msg, strlen(full_msg));
	free(full_msg);
	return (1);
}

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
			return print_pwd_error("cannot access parent directories: No such file or directory");
		else if (errno == EACCES)
			return print_pwd_error("Permission denied");
		else if (errno == ENAMETOOLONG)
			return print_pwd_error("File name too long");
		else
		{
			// For generic errors, use strerror(errno)
			char *msg = strerror(errno);
			return print_pwd_error(msg);
		}
	}
}
