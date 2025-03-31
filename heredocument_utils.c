/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:16:15 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 17:16:58 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_valid	check_exit_status(int status, int pipe_fds[2], t_cmd *cmd);
static t_valid	write_env(int pipe_fd, char **line, t_list *env_list);

int	write_expand_env(int pipe_fd, char *line, t_list *env_list)
{
	while (*line)
	{
		if (*line == '$')
		{
			if (line[1] == '?')
			{
				ft_putnbr_fd(g_last_exit_status, pipe_fd);
				line += 2;
				continue ;
			}
			else if (ft_isalpha(line[1]) || line[1] == '_')
			{
				if (write_env(pipe_fd, &line, env_list) == CRITICAL_ERROR)
					return (-1);
				continue ;
			}
		}
		if (write(pipe_fd, line++, 1) == -1)
			return (-1);
	}
	return (0);
}

static t_valid	write_env(int pipe_fd, char **line, t_list *env_list)
{
	t_env	*env;
	int		key_len;

	(*line)++;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		key_len = ft_strlen(env->key);
		if (ft_strncmp(env->key, *line, key_len) == 0
			&& !(ft_isalnum((*line)[key_len]) || (*line)[key_len] == '_'))
		{
			if (write(pipe_fd, env->value, ft_strlen(env->value)) == -1)
				return (CRITICAL_ERROR);
			*line += key_len;
			break ;
		}
		env_list = env_list->next;
	}
	if (env_list)
		return (VALID);
	while (ft_isalnum(**line) || **line == '_')
		(*line)++;
	if (write(pipe_fd, "", 0) == -1)
		return (CRITICAL_ERROR);
	return (VALID);
}

t_valid	wait_heredoc(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{
	int		status;
	pid_t	result;
	t_valid	is_valid;

	close_wrapper(&(pipe_fds[1]));
	result = waitpid(pid, &status, 0);
	while (result == -1 && errno == EINTR)
		result = waitpid(pid, &status, 0);
	if (result == -1)
	{
		perror("waitpid");
		close_wrapper(&(pipe_fds[0]));
		return (CRITICAL_ERROR);
	}
	is_valid = check_exit_status(status, pipe_fds, cmd);
	if (is_valid != VALID)
		return (is_valid);
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close_wrapper(&(pipe_fds[0]));
		return (CRITICAL_ERROR);
	}
	close_wrapper(&(pipe_fds[0]));
	return (VALID);
}

static t_valid	check_exit_status(int status, int pipe_fds[2], t_cmd *cmd)
{
	if (WIFSIGNALED(status))
	{
		g_last_exit_status = WTERMSIG(status) + 128;
		close_wrapper(&(pipe_fds[0]));
		if (cmd->backup_stdin != -1)
		{
			if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close_wrapper(&(cmd->backup_stdin));
				return (CRITICAL_ERROR);
			}
			close_wrapper(&(cmd->backup_stdin));
		}
		return (SIGINT_EXIT);
	}
	if (WIFEXITED(status))
	{
		g_last_exit_status = WEXITSTATUS(status);
		if (g_last_exit_status == 0)
			return (VALID);
		close_wrapper(&(pipe_fds[0]));
		return (CRITICAL_ERROR);
	}
	return (CRITICAL_ERROR);
}
