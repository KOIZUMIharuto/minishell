/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:56:39 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid execute_single_builtin(t_cmd *cmd, int (*builtin_func)(char **, t_list *), t_data data)
{
    int result;
    
    if (process_heredocs(cmd, data.env) == CRITICAL_ERROR)
        return (CRITICAL_ERROR);
    else if (process_heredocs(cmd, data.env) == INVALID ||process_heredocs(cmd, data.env) == SIGINT_EXIT)
    {
        g_last_exit_status = INVALID;
        return (INVALID);
    }

    if (handle_redirection(cmd, data.env)==CRITICAL_ERROR)
        return (CRITICAL_ERROR);
    if(restore_redirection(cmd)==CRITICAL_ERROR)
        return (CRITICAL_ERROR);

    setup_builtin_signals();
    result = builtin_func(cmd->cmd, data.env);
    if(restore_redirection(cmd)==CRITICAL_ERROR)
        return (CRITICAL_ERROR);
    if (ft_strcmp(cmd->cmd[0], "exit") != 0)
        g_last_exit_status = result;
    if (ft_strcmp(cmd->cmd[0], "exit") == 0)
    {
        if (result == -1) //koko
        {
            setup_interactive_signals();
            return (1);
        }
        return (EXIT_MAINT_LOOP);
    }
    setup_interactive_signals();
    return (VALID);
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
	int (*builtin_func)(char **, t_list *), t_data data, t_pipe_info *pipe_info)
{
	int		result;

	free(data.pids);
	data.pids = NULL;
	if (handle_redirection(cmd, data.env)==CRITICAL_ERROR)
	{
		free_data(data);
		exit(CRITICAL_ERROR);
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
		result = builtin_func(cmd->cmd, data.env);
		free_data(data);
		exit (result);
	}
	else
	{
		setup_child_signals();
		execute_cmd(cmd->cmd, data);
	}
}

static pid_t prepare_command(t_cmd *cmd, t_builtin *builtins, 
                            t_data data, t_pipe_info *pipe_info)
{
    int (*builtin_func)(char **, t_list *);
    pid_t pid;
    int builtin_index;
    
    builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		builtin_func = builtins[builtin_index].func;
    if (process_heredocs(cmd, data.env) == CRITICAL_ERROR)
        return (CRITICAL_ERROR);
    else if (process_heredocs(cmd, data.env) == INVALID ||process_heredocs(cmd, data.env) == SIGINT_EXIT)
    {
        g_last_exit_status = INVALID;
        return (INVALID);
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
		dup2(cmd->backup_stdin, STDIN_FILENO);
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
	}
    return pid;
}

pid_t	*execute_commands(t_builtin *builtins, t_data data, t_pipe_info *pipe_info)
{
    int i;
    

	i = 0;
	while (data.cmds[i])
		i++;
	data.pids = (pid_t *)malloc(sizeof(pid_t) * i);
    if (!data.pids)
    {
        perror("malloc");
        return (NULL);
    }
    i = 0;
    while (data.cmds[i])
    {
        if (setup_pipe(pipe_info, data.cmds[i + 1] != NULL) == CRITICAL_ERROR)
		{
			free(data.pids);
			return (NULL);
		}
            
        data.pids[i] = prepare_command(data.cmds[i], builtins, data, pipe_info);
        if (data.pids[i] == -1)
		{
			free(data.pids);
			return (NULL);
		}
        manage_pipes(pipe_info);
        i++;
    }
    return (data.pids);
}



