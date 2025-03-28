/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:16:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 20:04:28 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	check_pipeline(t_cmd **cmds, int *cmd_count)
{
    *cmd_count = 0;
    while (cmds[*cmd_count])
        (*cmd_count)++;
        
    return (*cmd_count > 1);
}

static t_valid try_execute_as_builtin(t_cmd *cmd, t_builtin *builtins,t_data data)
{
	int		builtin_index; 
	t_valid	(*builtin_func)(char **, t_list *);

	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
    builtin_func = NULL;
	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
    {
		builtin_func = builtins[builtin_index].func;
		return (execute_single_builtin(cmd, builtin_func, data));
    }
	return (CMD_EXTERNAL);
}

int wait_for_children(pid_t *pids, int cmd_count)
{
    int status;
    int i;

    i = 0;
    while (i < cmd_count)
    {
        
        int result;
        while ((result = waitpid(pids[i], &status, 0)) == -1 && errno == EINTR)
            ;
        // {
        //     perror("1 waitpid interrupted by signal, retrying...");
        // }
        if (result == -1)
        {
            perror("waitpid");
            continue;
        }
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                g_last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
            {
                g_last_exit_status = WTERMSIG(status) + 128;
                // if (WTERMSIG(status) == SIGINT)
                    // printf("\nSIGINT detected. Terminating pipeline.\n");
            }
        }
        i++;
    }
    return g_last_exit_status;
}

t_valid execute_pipeline(t_cmd **cmds, t_builtin *builtins, t_list *env)
{
	t_data	data;
    int cmd_count;
    t_pipe_info pipe_info;
    int result;
    
    pipe_info.prev_fd = -1;
	data.env = env;
	data.cmds = cmds;
	data.pids = NULL;
    data.pids = NULL;
    
    if (!check_pipeline(cmds, &cmd_count))
    {
        if (try_execute_as_builtin(cmds[0], builtins,data) != CMD_EXTERNAL)
		{
			free_cmds(cmds);
            return (VALID);
		}
    }
    data.pids = execute_commands(builtins, data, &pipe_info);
    
	free_cmds(cmds);
	if (!data.pids)
	{
		return (EXIT_FAILURE);
	}

    wait_for_children(data.pids, cmd_count);
    
    free(data.pids);
    setup_interactive_signals();
    return g_last_exit_status;
}
