/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:07:49 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 13:51:44 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

static int	write_expand_env(int pipe_fd, char *line, t_list *env_list)
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

static t_valid	setup_heredoc_pipe(int pipe_fds[2], t_cmd *cmd)
{
	bool	is_valid;

	is_valid = true;
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (CRITICAL_ERROR);
	}
	if (cmd->backup_stdin == -1)
	{
		cmd->backup_stdin = dup(STDIN_FILENO);
		if (cmd->backup_stdin == -1)
			is_valid = perror_bool("dup");
	}
	else
	{
		if (cmd->backup_stdin == -1
			|| dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
			is_valid = perror_bool("dup2");
	}
	if (is_valid)
		return (VALID);
	close_wrapper(&(pipe_fds[0]));
	close_wrapper(&(pipe_fds[1]));
	return (CRITICAL_ERROR);
}

static void	process_heredoc_child(int pipe_fds[2], t_rdrct *rdrct, t_list *env)
{
	char	*line;

	close_wrapper(&(pipe_fds[0]));
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, rdrct->file[0]) == 0)
		{
			free(line);
			break ;
		}
		if (((!rdrct->is_quoted && ft_strlen(line) > 0
					&& write_expand_env(pipe_fds[1], line, env) == -1)
				|| (rdrct->is_quoted && ft_strlen(line) > 0
					&& write(pipe_fds[1], line, ft_strlen(line)) == -1))
			|| write(pipe_fds[1], "\n", 1) == -1)
		{
			free(line);
			exit(perror_int("write"));
		}
		free(line);
	}
	close_wrapper(&(pipe_fds[1]));
	exit(0);
}

t_valid	setup_parent_process(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{
	int		status;
	pid_t	result;

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
		if (g_last_exit_status != 0)
		{
			close_wrapper(&(pipe_fds[0]));
			return (CRITICAL_ERROR);
		}
	}
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close_wrapper(&(pipe_fds[0]));
		return (CRITICAL_ERROR);
	}
	close_wrapper(&(pipe_fds[0]));
	return (VALID);
}

t_valid	process_heredocs(t_cmd *cmd, t_list *env)
{
	int		i;
	int		pipe_fds[2];
	pid_t	pid;
	t_valid	is_valid;

	i = -1;
	while (cmd->rdrcts[++i])
	{
		if (cmd->rdrcts[i]->type != HEREDOCUMENT)
			continue ;
		if (setup_heredoc_pipe(pipe_fds, cmd) == CRITICAL_ERROR)
			return (CRITICAL_ERROR);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			close_wrapper(&(pipe_fds[0]));
			close_wrapper(&(pipe_fds[1]));
			return (CRITICAL_ERROR);
		}
		if (pid == 0)
		{
			close_wrapper(&(cmd->backup_stdin));
			process_heredoc_child(pipe_fds, cmd->rdrcts[i], env);
		}
		is_valid = setup_parent_process(pipe_fds, cmd, pid);
		setup_interactive_signals();
		if (is_valid == SIGINT_EXIT)
		{
			if (cmd->backup_stdin != -1 && dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close_wrapper(&(cmd->backup_stdin));
				return (CRITICAL_ERROR);
			}
			close_wrapper(&(cmd->backup_stdin));
			return (SIGINT_EXIT);
		}
	}
	return (VALID);
}
