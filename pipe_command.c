/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/21 20:13:10 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	process_heredocs(t_cmd *cmd)
{
	int	j;
    int original_stdin = -1;

    if (cmd->backup_stdin == -1)
    {
        cmd->backup_stdin = dup(STDIN_FILENO);
        if (cmd->backup_stdin == -1)
            return (-1);
    }
    original_stdin = dup(cmd->backup_stdin);
    if (original_stdin == -1)
    {
        perror("dup");
        return (-1);
    }
	j = 0;
	while (cmd->rdrcts[j])
	{
		if (cmd->rdrcts[j]->type == HEREDOCUMENT)
		{
            dup2(original_stdin, STDIN_FILENO);
			if (handle_heredocument(cmd->rdrcts[j]->file[0], cmd) == -1)
            {
                close(original_stdin);
				return (-1);
            }
		}
		j++;
	}
    close(original_stdin);
	return (0);
}


int execute_single_builtin(t_cmd *cmd, t_builtin *builtins, 
                           int builtin_index, t_list *env)
{
    int result;
    
    setup_builtin_signals();
    if (process_heredocs(cmd) == -1)
        return (1);
    if (handle_redirection(cmd))
    {
        restore_redirection(cmd);
        g_last_exit_status = 1;
        return (1);
    }
    
    result = builtins[builtin_index].func(cmd->cmd, env);
    restore_redirection(cmd);
    if (ft_strcmp(cmd->cmd[0], "exit") != 0)
        g_last_exit_status = result;
    
    
    if (ft_strcmp(cmd->cmd[0], "exit") == 0)
    {
        if (result == -1)
        {
            setup_interactive_signals();
            return (1);
        }
        return (-42);
    }
    setup_interactive_signals();
    return result;
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
	int (*builtin_func)(char **, t_list *), t_list *env, t_pipe_info *pipe_info)
{
	int		result;

	if (process_heredocs(cmd) == -1 || handle_redirection(cmd))
	{
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	if (!has_input_redirection(cmd))
		handle_pipe_input(pipe_info);
	if (!has_output_redirection(cmd))
		handle_pipe_output(pipe_info);
	if (!cmd->cmd[0])
	{
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	if (builtin_func)
	{
		setup_builtin_signals();
		result = builtin_func(cmd->cmd, env);
		free_cmd(cmd);
		exit (result);
	}
	else
	{
		setup_child_signals();
		execute_cmd(cmd->cmd, env);
	}
}

static pid_t prepare_command(t_cmd *cmd, t_builtin *builtins, 
                            t_list *env, t_pipe_info *pipe_info)
{
    int (*builtin_func)(char **, t_list *);
    pid_t pid;
    int builtin_index;
    
    builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		builtin_func = builtins[builtin_index].func;
    setup_exec_signals();
    pid = fork();
    if (pid == 0)
        execute_command_in_child(cmd, builtin_func, env, pipe_info);
    free_cmd(cmd);
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    return pid;
}

int execute_commands(t_cmd **cmds, t_builtin *builtins, t_list *env, 
                    pid_t *pids, t_pipe_info *pipe_info)
{
    int i;
    pid_t pid;
    
    i = 0;
    while (cmds[i])
    {
        if (setup_pipe(pipe_info, cmds[i + 1] != NULL) == -1)
            return -1;
            
        pid = prepare_command(cmds[i], builtins, env, pipe_info);
        if (pid == -1)
            return -1;
            
        pids[i] = pid;
        manage_pipes(pipe_info);
        i++;
    }
	free(cmds);
    return i;
}



