/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 12:00:23 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid execute_single_builtin(t_cmd *cmd, t_valid (*builtin_func)(char **, t_list *), t_data data)	//ok
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
	setup_interactive_signals();
	if(restore_redirection(cmd)==CRITICAL_ERROR)
		return (CRITICAL_ERROR);
	if (ft_strcmp(cmd->cmd[0], "exit") == 0)
	{
		if (is_valid == INVALID)
		{
			g_last_exit_status = 1;
			return (INVALID);
		}
		return (EXIT_MAINT_LOOP);
	}
	if (is_valid == VALID)
		g_last_exit_status = 0;
	else
		g_last_exit_status = 1;
	return (is_valid);
}

bool has_input_redirection(t_cmd *cmd)
{
    int j = 0;
    
    while (cmd->rdrcts[j])
    {
        if (cmd->rdrcts[j]->type == INPUT_RDRCT || 
            cmd->rdrcts[j]->type == HEREDOCUMENT)
            return true;
        j++;
    }
    return false;
}

bool has_output_redirection(t_cmd *cmd)
{
    int j = 0;
    while (cmd->rdrcts[j])
    {
        if (cmd->rdrcts[j]->type == OVERWRITE_RDRCT || 
            cmd->rdrcts[j]->type == APPEND_RDRCT)
            return true;
        j++;
    }
    return false;
}

void	execute_command_in_child(t_cmd *cmd,
	t_valid (*builtin_func)(char **, t_list *), t_data data, t_pipe_info *pipe_info)	//ok
{
	t_valid is_valid;

	free(data.pids);
	data.pids = NULL;
	is_valid = handle_redirection(cmd, data.env);
	if (is_valid != VALID)
	{
		free_data(data);
		exit(is_valid);
	}
	if (!has_input_redirection(cmd))
    {
		if(handle_pipe_input(pipe_info) == CRITICAL_ERROR)
        {
            free_data(data);
            exit(CRITICAL_ERROR);
        }
    }
	if (!has_output_redirection(cmd))
    {
        if(handle_pipe_output(pipe_info) == CRITICAL_ERROR)
		{
            free_data(data);
            exit(CRITICAL_ERROR);
        }
    }
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

static pid_t prepare_command(t_cmd *cmd, t_builtin *builtins, 
                            t_data data, t_pipe_info *pipe_info)	//ok
{
    t_valid (*builtin_func)(char **, t_list *);
    pid_t pid;
    int builtin_index;
	t_valid	is_valid;
    
    builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		builtin_func = builtins[builtin_index].func;
	is_valid = process_heredocs(cmd, data.env);
    if (is_valid != VALID)
	{
		// printf("heredoc invalid : [%d]\n", is_valid);
        return (is_valid);
	}
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (CRITICAL_ERROR);
    }
    if (pid == 0)
    {
        setup_exec_signals();
        execute_command_in_child(cmd, builtin_func, data, pipe_info);
    }
	if (cmd->backup_stdin != -1)
	{
		if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(cmd->backup_stdin);
			cmd->backup_stdin = -1;
			return (CRITICAL_ERROR);
		}
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
	}
    return (pid);
}

t_valid	execute_commands(t_builtin *builtins, t_data *data, t_pipe_info *pipe_info)	//ok
{
    int i;
	t_valid	is_valid;

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
            
        is_valid = prepare_command(data->cmds[i], builtins, *data, pipe_info);
        if (is_valid < 0)
		{
			free(data->pids);
			return (is_valid);
		}
		data->pids[i] = is_valid;
        manage_pipes(pipe_info);
        i++;
    }
    return (VALID);
}



