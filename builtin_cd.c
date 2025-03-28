/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:35 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 20:50:11 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		update_pwd(t_list *env_list);
static t_valid	perror_cd(char *arg, int errnum, t_valid is_valid);

t_valid	builtin_cd(char **cmd, t_list *env)
{
	struct stat	path_stat;
	t_env		*home_dir;
	char		*directory;

	if (cmd[1] && cmd[2])
		return (error_msg("cd", "too many arguments", INVALID));
	if (cmd[1] == NULL)
	{
		home_dir = env_get(env, "HOME", false);
		if (!home_dir)
			return (error_msg("cd", "HOME not set", INVALID));
		directory = home_dir->value;
	}
	else
		directory = cmd[1];
	if (stat(directory, &path_stat) != 0)
		return (perror_cd(directory, errno, INVALID));
	if (!S_ISDIR(path_stat.st_mode))
		return (perror_cd(directory, ENOTDIR, INVALID));
	if (chdir(directory) != 0)
		return (perror_cd(directory, errno, CRITICAL_ERROR));
	if (!update_pwd(env))
		return (CRITICAL_ERROR);
	return (VALID);
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

static t_valid	perror_cd(char *arg, int errnum, t_valid is_valid)
{
	if (!print_msg("minishell: cd: ", STDERR_FILENO)
		|| !print_msg(arg, STDERR_FILENO)
		|| !print_msg(": ", STDERR_FILENO)
		|| !print_msg(strerror(errnum), STDERR_FILENO)
		|| !print_msg("\n", STDERR_FILENO))
	{
		return (CRITICAL_ERROR);
	}
	return (is_valid);
}
