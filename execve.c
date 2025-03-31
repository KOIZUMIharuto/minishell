/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 03:20:59 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 12:43:42 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*concat_path_cmd(char *dir, char *cmd)
{
	int		total_len;
	char	*cmd_path;

	total_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	cmd_path = malloc(total_len);
	if (!cmd_path)
		return (NULL);
	ft_strlcpy(cmd_path, dir, total_len);
	ft_strlcat(cmd_path, "/", total_len);
	ft_strlcat(cmd_path, cmd, total_len);
	return (cmd_path);
}

bool	find_cmd_path(char **cmd_path, char *cmd, t_env *path_env)
{
	char		**path_dirs;
	int			i;
	struct stat	sb;

	path_dirs = ft_split(path_env->value, ':');
	if (!path_dirs)
		return (false);
	i = 0;
	while (path_dirs[i])
	{
		*cmd_path = concat_path_cmd(path_dirs[i], cmd);
		if (stat(*cmd_path, &sb) == 0)
		{
			free_double_pointor(path_dirs);
			return (true);
		}
		free(*cmd_path);
		i++;
	}
	free_double_pointor(path_dirs);
	*cmd_path = NULL;
	return (true);
}

void	check_path(char **cmd, char *cmd_path, t_data data)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == -1)
	{
		(void)error_msg(cmd_path, strerror(errno), INVALID);
		if (cmd_path != cmd[0])
			free(cmd_path);
		free_data(data);
		if (errno == ENOENT)
			exit(127);
		exit(126);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		(void)error_msg(cmd_path, "Is a directory", INVALID);
		if (cmd_path != cmd[0])
			free(cmd_path);
		free_data(data);
		exit(126);
	}
}

void	convert_env_execute(char **cmd, char *cmd_path, t_data data)
{
	char		**env_array;

	env_array = convert_env_list_to_array(data.env);
	if (!env_array)
	{
		perror("malloc");
		if (cmd_path != cmd[0])
			free(cmd_path);
		free_data(data);
		exit(CRITICAL_ERROR);
	}
	if (execve(cmd_path, cmd, env_array) == -1)
	{
		(void)error_msg(cmd_path, strerror(errno), INVALID);
		if (cmd_path != cmd[0])
			free(cmd_path);
		free_data(data);
		free_double_pointor(env_array);
		if (errno == ENOENT)
			exit(127);
		exit(126);
	}
}

void	execute_cmd(char **cmd, t_data data)
{
	t_env		*path_env;
	char		*cmd_path;

	path_env = env_get(data.env, "PATH", false);
	if (!cmd[0][0])
		cmd_path = NULL;
	else if (ft_strchr(cmd[0], '/') || !path_env || !path_env->value)
		cmd_path = ft_strdup(cmd[0]);
	else if (!find_cmd_path(&cmd_path, cmd[0], path_env))
	{
		free_data(data);
		perror("malloc");
		exit(CRITICAL_ERROR);
	}
	if (!cmd_path)
	{
		(void)error_msg(cmd[0], "command not found", CRITICAL_ERROR);
		free_data(data);
		exit(127);
	}
	check_path(cmd, cmd_path, data);
	convert_env_execute(cmd, cmd_path, data);
}
