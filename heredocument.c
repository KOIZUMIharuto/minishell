/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:07:49 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 23:41:06 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_valid	setup_heredoc_pipe(int pipe_fds[2], t_cmd *cmd);
static void		process_heredoc_child(int pipe_fds[2],
					t_rdrct *rdrct, t_list *env);
static t_valid	do_heredoc(int pipe_fds[2], t_cmd *cmd, pid_t pid);

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
		if (pid == 0)
		{
			close_wrapper(&(cmd->backup_stdin));
			process_heredoc_child(pipe_fds, cmd->rdrcts[i], env);
		}
		is_valid = do_heredoc(pipe_fds, cmd, pid);
		if (is_valid != VALID)
			return (is_valid);
	}
	return (VALID);
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
					&& !print_msg(line, pipe_fds[1])))
			|| !print_msg("\n", pipe_fds[1]))
		{
			free(line);
			exit(1);
		}
		free(line);
	}
	close_wrapper(&(pipe_fds[1]));
	exit(0);
}

static t_valid	do_heredoc(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{
	t_valid	is_valid;

	if (pid == -1)
	{
		perror("fork");
		close_wrapper(&(pipe_fds[0]));
		close_wrapper(&(pipe_fds[1]));
		return (CRITICAL_ERROR);
	}
	is_valid = wait_heredoc(pipe_fds, cmd, pid);
	setup_interactive_signals();
	if (is_valid == SIGINT_EXIT)
	{
		if (cmd->backup_stdin != -1
			&& dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close_wrapper(&(cmd->backup_stdin));
			return (CRITICAL_ERROR);
		}
		close_wrapper(&(cmd->backup_stdin));
		return (SIGINT_EXIT);
	}
	return (VALID);
}
