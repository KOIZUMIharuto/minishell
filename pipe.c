/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:16:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 20:34:32 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool check_pipeline(t_cmd **cmds, int *cmd_count)
{
    bool is_pipeline;
    
    *cmd_count = 0;
    while (cmds[*cmd_count])
        (*cmd_count)++;
        
    is_pipeline = (*cmd_count > 1);
    return is_pipeline;
}

static int try_execute_as_builtin(t_cmd *cmd, t_builtin *builtins, t_list *env)
{
	int	builtin_index;

	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
		return (execute_single_builtin(cmd, builtins, builtin_index, env));
	return (-1);
}

int wait_for_children(pid_t *pids, int cmd_count)
{
    int status;
    int i;
    
    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], &status, 0);
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                g_last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                g_last_exit_status = WTERMSIG(status) + 128;
        }
        i++;
    }
    
    return g_last_exit_status;
}

int execute_pipeline(t_cmd **cmds, t_builtin *builtins, t_list *env)
{
    int cmd_count;
    pid_t *pids;
    t_pipe_info pipe_info;
    int result;
    
    if (!check_pipeline(cmds, &cmd_count))
    {
        result = try_execute_as_builtin(cmds[0], builtins, env);
        if (result != -1)
            return result;
    }
    
    pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
        return (EXIT_FAILURE);
        
    pipe_info.prev_fd = -1;
    execute_commands(cmds, builtins, env, pids, &pipe_info);
    
    wait_for_children(pids, cmd_count);
    
    free(pids);
    setup_interactive_signals();
    return g_last_exit_status;
}
