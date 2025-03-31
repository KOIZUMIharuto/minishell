/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 17:25:10 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	after_builtin(t_cmd *cmd, t_valid is_valid)
{
	setup_interactive_signals();
	if (restore_redirection(cmd) == CRITICAL_ERROR)
		return (CRITICAL_ERROR);
	if (ft_strcmp(cmd->cmd[0], "exit") == 0)
	{
		if (is_valid != INVALID)
			return (EXIT_MAINT_LOOP);
		g_last_exit_status = 1;
		return (INVALID);
	}
	if (is_valid == VALID)
		g_last_exit_status = 0;
	else
		g_last_exit_status = 1;
	return (is_valid);
}

t_valid	exec_single_builtin(t_cmd *cmd,
	t_valid (*builtin_func)(char **, t_list *), t_data data)
{
	t_valid	is_valid;

	is_valid = process_heredocs(cmd, data.env);
	if (is_valid != VALID)
		return (is_valid);
	is_valid = handle_redirection(cmd, data.env);
	if (is_valid != VALID)
	{
		g_last_exit_status = 1;
		restore_redirection(cmd);
		return (is_valid);
	}
	setup_builtin_signals();
	is_valid = builtin_func(cmd->cmd, data.env);
	is_valid = after_builtin(cmd, is_valid);
	return (is_valid);
}

bool	has_input_redirection(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->rdrcts[i])
	{
		if (cmd->rdrcts[i]->type == INPUT_RDRCT
			|| cmd->rdrcts[i]->type == HEREDOCUMENT)
			return (true);
		i++;
	}
	return (false);
}

bool	has_output_redirection(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->rdrcts[i])
	{
		if (cmd->rdrcts[i]->type == OVERWRITE_RDRCT
			|| cmd->rdrcts[i]->type == APPEND_RDRCT)
			return (true);
		i++;
	}
	return (false);
}

void	exec_command_redirect(t_cmd *cmd, t_pipe_info *pipe_info, t_data data)
{
	t_valid	is_valid;

	is_valid = handle_redirection(cmd, data.env);
	if (is_valid != VALID)
	{
		free_data(data);
		exit(is_valid);
	}
	if (!has_input_redirection(cmd))
	{
		if (handle_pipe_input(pipe_info) == CRITICAL_ERROR)
		{
			free_data(data);
			exit(CRITICAL_ERROR);
		}
	}
	if (!has_output_redirection(cmd))
	{
		if (handle_pipe_output(pipe_info) == CRITICAL_ERROR)
		{
			free_data(data);
			exit(CRITICAL_ERROR);
		}
	}
}

void	exec_command_in_child(t_cmd *cmd, t_pipe_info *pipe_info,
	t_valid (*builtin_func)(char **, t_list *), t_data data)
{
	t_valid	is_valid;

	free(data.pids);
	data.pids = NULL;
	exec_command_redirect(cmd, pipe_info, data);
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
		exit (is_valid);
	}
	else
	{
		setup_child_signals();
		execute_cmd(cmd->cmd, data);
	}
}

static int prepare_command(t_cmd *cmd, t_builtin *builtins, 
	t_data data, t_pipe_info *pipe_info)
{
	t_valid	builtin_func(char **, t_list *);	//fix
	pid_t	pid;
	int		builtin_index;
	t_valid	is_valid;

	builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		builtin_func = builtins[builtin_index].func;
	is_valid = process_heredocs(cmd, data.env);
	if (is_valid != VALID)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_exec_signals();
		exec_command_in_child(cmd, pipe_info, builtin_func, data);
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

t_valid	exec_commands(t_builtin *builtins, t_data *data, t_pipe_info *pipe_info)
{
	int	i;
	int	pid;

	i = 0;
	while (data->cmds[i])
		i++;
	data->pids = (pid_t *)malloc(sizeof(pid_t) * i);
	if (!data->pids)
	{
		perror("malloc");
		return (CRITICAL_ERROR);
	}
	i = 0;
	while (data->cmds[i])
	{
		if (setup_pipe(pipe_info, data->cmds[i + 1] != NULL) == CRITICAL_ERROR)
		{
			free(data->pids);
			return (CRITICAL_ERROR);
		}
		pid = prepare_command(data->cmds[i], builtins, *data, pipe_info);
		if (pid < 0)
		{
			free(data->pids);
			return (pid);
		}
		data->pids[i] = pid;
		manage_pipes(pipe_info);
		i++;
	}
	return (VALID);
}
