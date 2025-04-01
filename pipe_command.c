/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/04/01 19:41:39 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_valid	after_builtin(t_cmd *cmd, t_valid is_valid);
static bool		has_input_redirection(t_cmd *cmd);
static bool		has_output_redirection(t_cmd *cmd);

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

static t_valid	after_builtin(t_cmd *cmd, t_valid is_valid)
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

void	exec_cmd_redirect(t_cmd *cmd, t_pipe_info *pipe_info, t_data data)
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

static bool	has_input_redirection(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redirects[i])
	{
		if (cmd->redirects[i]->type == INPUT_RDRCT
			|| cmd->redirects[i]->type == HEREDOCUMENT)
			return (true);
		i++;
	}
	return (false);
}

static bool	has_output_redirection(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redirects[i])
	{
		if (cmd->redirects[i]->type == OVERWRITE_RDRCT
			|| cmd->redirects[i]->type == APPEND_RDRCT)
			return (true);
		i++;
	}
	return (false);
}
