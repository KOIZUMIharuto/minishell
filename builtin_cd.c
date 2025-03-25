/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:35 by shiori            #+#    #+#             */
/*   Updated: 2025/03/25 17:12:45 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	update_pwd(t_list *env_list);
static int	perror_cd(char *arg, int errnum);

int	builtin_cd(char **cmd, t_list *env)
{
	struct stat	path_stat;
	t_env		*home_dir;
	char		*directory;

	if (cmd[1] && cmd[2])
		return (error_msg("cd", "too many arguments"));
	if (cmd[1] == NULL)
	{
		home_dir = env_get(env, "HOME", false);
		if (!home_dir)
			return (error_msg("cd", "HOME not set"));
		directory = home_dir->value;
	}
	else
		directory = cmd[1];
	if (stat(directory, &path_stat) != 0)
		return (perror_cd(directory, errno));
	if (!S_ISDIR(path_stat.st_mode))
		return (perror_cd(directory, ENOTDIR));
	if (chdir(directory) != 0)
		return (perror_cd(directory, errno));
	if (!update_pwd(env))
		return (1);
	return (0);
}

static bool	update_pwd(t_list *env_list)
{
	t_env	*env_pwd;
	t_env	*env_oldpwd;
	char	*new_pwd;

	env_pwd = env_get(env_list, "PWD", true);
	env_oldpwd = env_get(env_list, "OLDPWD", false);
	new_pwd = get_pwd();
	if (!new_pwd)
		return (false);
	if (env_oldpwd)
	{
		free(env_oldpwd->value);
		env_oldpwd->value = env_pwd->value;
	}
	env_pwd->value = new_pwd;
	return (true);
}

static int	perror_cd(char *arg, int errnum)
{
	if (!print_msg("minishell: cd: ", STDERR_FILENO)
		|| !print_msg(arg, STDERR_FILENO)
		|| !print_msg(": ", STDERR_FILENO)
		|| !print_msg(strerror(errnum), STDERR_FILENO)
		|| !print_msg("\n", STDERR_FILENO))
	{
		g_last_exit_status = EXIT_FAILURE;
		perror("write");
	}
	return (1);
}
