/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:57:09 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/04/01 19:41:24 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_valid	exec_cmd(t_data *data, t_builtin *builtins,
					t_pipe_info *pipe_info);
static pid_t	prepare_cmd(t_cmd *cmd, t_builtin *builtins, t_data data,
					t_pipe_info *pipe_info);
static pid_t	fork_exec_process(t_cmd *cmd, t_pipe_info *pipe_info,
					t_valid (*builtin_func)(char **, t_list *), t_data data);
static void		exec_cmd_in_child(t_cmd *cmd, t_pipe_info *pipe_info,
					t_valid (*builtin_func)(char **, t_list *), t_data data);

t_valid	exec_cmds(t_builtin *builtins, t_data *data, t_pipe_info *pipe_info)
{
	int	i;

	i = 0;
	while (data->cmds[i])
		i++;
	data->pids = (pid_t *)malloc(sizeof(pid_t) * i);
	if (!data->pids)
	{
		perror("malloc");
		return (CRITICAL_ERROR);
	}
	return (exec_cmd(data, builtins, pipe_info));
}

static t_valid	exec_cmd(t_data *data, t_builtin *builtins,
					t_pipe_info *pipe_info)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (data->cmds[i])
	{
		if (setup_pipe(pipe_info, data->cmds[i + 1] != NULL) == CRITICAL_ERROR)
		{
			free(data->pids);
			return (CRITICAL_ERROR);
		}
		pid = prepare_cmd(data->cmds[i], builtins, *data, pipe_info);
		if (pid < 0)
		{
			free(data->pids);
			return ((t_valid)pid);
		}
		data->pids[i] = pid;
		manage_pipes(pipe_info);
		i++;
	}
	return (VALID);
}

static pid_t	prepare_cmd(t_cmd *cmd, t_builtin *builtins, t_data data,
					t_pipe_info *pipe_info)
{
	int		builtin_index;
	t_valid	is_valid;

	t_valid((*builtin_func)(char **, t_list *));
	builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		builtin_func = builtins[builtin_index].func;
	is_valid = process_heredocs(cmd, data.env);
	if (is_valid != VALID)
		return (is_valid);
	return (fork_exec_process(cmd, pipe_info, builtin_func, data));
}

static pid_t	fork_exec_process(t_cmd *cmd, t_pipe_info *pipe_info,
					t_valid (*builtin_func)(char **, t_list *), t_data data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_exec_signals();
		exec_cmd_in_child(cmd, pipe_info, builtin_func, data);
	}
	if (cmd->backup_stdin != -1)
	{
		if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close_wrapper(&(cmd->backup_stdin));
			return (-1);
		}
		close_wrapper(&(cmd->backup_stdin));
	}
	return (pid);
}

static void	exec_cmd_in_child(t_cmd *cmd, t_pipe_info *pipe_info,
				t_valid (*builtin_func)(char **, t_list *), t_data data)
{
	t_valid	is_valid;

	free(data.pids);
	data.pids = NULL;
	exec_cmd_redirect(cmd, pipe_info, data);
	if (!cmd->cmd[0])
	{
		free_data(data);
		exit(EXIT_SUCCESS);
	}
	if (builtin_func)
	{
		setup_builtin_signals();
		is_valid = builtin_func(cmd->cmd, data.env);
		free_data(data);
		exit(is_valid);
	}
	else
	{
		setup_child_signals();
		execute_cmd(cmd->cmd, data);
	}
}
