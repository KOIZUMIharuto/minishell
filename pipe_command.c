/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/27 16:52:26 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	process_heredocs(t_cmd *cmd, t_rdrct *redirect, t_list *env)
{
	// int	j;
    int original_stdin = -1;
    int result;

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
    
    // 標準入力を元に戻す
    dup2(original_stdin, STDIN_FILENO);
    printf("original_stdin: %d\n", original_stdin);
    printf("backup_stdin: %d\n", cmd->backup_stdin);
    // ヒアドキュメント処理
    result = handle_heredocument(redirect, cmd, env);
    dup2(original_stdin, STDIN_FILENO);
    // restore_redirection(cmd);
    setup_interactive_signals();
    
    // 後処理
    close(original_stdin);
    
    return result;
}


int execute_single_builtin(t_cmd *cmd, t_builtin *builtins, 
                           int builtin_index, t_list *env)
{
    int result;
    
    setup_builtin_signals();
    // if (process_heredocs(cmd, env) == -1)
    //     return (1);
    if (handle_redirection(cmd, env))
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
	int (*builtin_func)(char **, t_list *), t_data data, t_pipe_info *pipe_info)
{
	int		result;

	free(data.pids);
	data.pids = NULL;
	// if (process_heredocs(cmd, data.env) == -1 || handle_redirection(cmd, data.env))
	if (handle_redirection(cmd, data.env))
	{
		free_data(data);
		exit(EXIT_FAILURE);
	}
	if (!has_input_redirection(cmd))
		handle_pipe_input(pipe_info);
	if (!has_output_redirection(cmd))
		handle_pipe_output(pipe_info);
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
    setup_exec_signals();
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
        execute_command_in_child(cmd, builtin_func, data, pipe_info);
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
        return (NULL);
    i = 0;
    while (data.cmds[i])
    {
        if (setup_pipe(pipe_info, data.cmds[i + 1] != NULL) == -1)
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



