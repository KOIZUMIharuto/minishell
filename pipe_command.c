/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 04:37:51 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int execute_single_builtin(t_cmd *cmd, t_builtin *builtins, 
                           int builtin_index, t_list *env)
{
    int result;
    
    setup_builtin_signals();
    
    if (process_heredocs(cmd) == -1)
        return EXIT_FAILURE;
    
    if (handle_redirection(cmd) == -1)
        return EXIT_FAILURE;
    
    result = builtins[builtin_index].func(cmd->cmd, env);
    if (ft_strcmp(cmd->cmd[0], "exit") != 0)
        g_last_exit_status = result;
    
    restore_redirection(cmd);
    
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

void execute_command_in_child(t_cmd *cmd, int (*builtin_func)(char **, t_list *), 
                             t_list *env, t_pipe_info *pipe_info)
{
    setup_child_signals();
    
    if (process_heredocs(cmd) == -1)
        exit(EXIT_FAILURE);

    if (handle_redirection(cmd) == -1)
        exit(EXIT_FAILURE);

    if (pipe_info->prev_fd != -1)
    {
        dup2(pipe_info->prev_fd, STDIN_FILENO);
        close(pipe_info->prev_fd);
    }
    if (pipe_info->has_next)
    {
        dup2(pipe_info->pipe_fds[1], STDOUT_FILENO);
        close(pipe_info->pipe_fds[1]);
        close(pipe_info->pipe_fds[0]);
    }

    if (builtin_func)
    {
        int result = builtin_func(cmd->cmd, env);
        exit(result);  
    }
    else
    {
        execute_cmd(cmd->cmd, env);
    }
}

static pid_t prepare_command(t_cmd *cmd, t_builtin *builtins, 
                            t_list *env, t_pipe_info *pipe_info)
{
    int (*builtin_func)(char **, t_list *);
    pid_t pid;
    
    builtin_func = NULL;
    if (get_builtin_index(builtins, cmd->cmd[0]) >= 0)
        builtin_func = builtins[get_builtin_index(builtins, cmd->cmd[0])].func;
    
    setup_exec_signals();
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    
    if (pid == 0)
        execute_command_in_child(cmd, builtin_func, env, pipe_info);
    
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
        manage_parent_pipes(pipe_info);
        i++;
    }
    
    return i;
}