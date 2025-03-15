/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:35 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 01:28:33 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	perror_cd(char *command, char *arg, int errnum);

int	builtin_cd(char **cmd, t_list *env)
{
	struct stat	path_stat;
	t_env		*home_dir;
	char		*directory;

	if (cmd[1] == NULL)
	{
		home_dir = env_get(env, "HOME");
		if (!home_dir)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
		directory = home_dir->value;
	}
	else
		directory = cmd[1];
	if (stat(directory, &path_stat) != 0)
		return (perror_cd("cd", directory, errno));
	if (!S_ISDIR(path_stat.st_mode))
		return (perror_cd("cd", directory, ENOTDIR));
	if (chdir(directory) != 0)
		return (perror_cd("cd", directory, errno));
	return (0);
}

static int	perror_cd(char *command, char *arg, int errnum)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}
