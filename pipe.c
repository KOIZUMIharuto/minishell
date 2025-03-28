/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:16:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 22:49:11 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	check_pipeline(t_cmd **cmds, int *cmd_count)	//ok
{
    *cmd_count = 0;
    while (cmds[*cmd_count])
        (*cmd_count)++;
        
    return (*cmd_count > 1);
}

static t_valid try_execute_as_builtin(t_cmd *cmd, t_builtin *builtins,t_data data)	//ok
{
	int		builtin_index; 
	t_valid	(*builtin_func)(char **, t_list *);

	builtin_index = get_builtin_index(builtins, cmd->cmd[0]);
	if (builtin_index >= 0)
    {
		builtin_func = builtins[builtin_index].func;
		return (execute_single_builtin(cmd, builtin_func, data));
    }
	return (CMD_EXTERNAL);
}

void wait_for_children(pid_t *pids, int cmd_count)
{
    int status;
    int i;
	int result;

    i = -1;
    while (++i < cmd_count)
    {
        
		result = waitpid(pids[i], &status, 0);
        while (result == -1 && errno == EINTR)
		{
			printf("wait restart\n");
            result = waitpid(pids[i], &status, 0);
		}
        if (result == -1)
        {
            perror("waitpid?");
            continue;
        }
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                g_last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                g_last_exit_status = WTERMSIG(status) + 128;
        }
    }
}

t_valid execute_pipeline(t_cmd **cmds, t_builtin *builtins, t_list *env)	//ok
{
	t_data	data;
    int cmd_count;
    t_pipe_info pipe_info;
	t_valid	is_valid;

    pipe_info.prev_fd = -1;
	data.env = env;
	data.cmds = cmds;
	data.pids = NULL;
    data.pids = NULL;
    
    if (!check_pipeline(cmds, &cmd_count))
    {
		is_valid = try_execute_as_builtin(cmds[0], builtins,data);
        if (is_valid != CMD_EXTERNAL)
		{
			free_cmds(cmds);
            return (is_valid);
		}
    }
    is_valid = execute_commands(builtins, &data, &pipe_info);
	free_cmds(cmds);
	if (is_valid != VALID)
		return (is_valid);

    wait_for_children(data.pids, cmd_count);
    
    free(data.pids);
    setup_interactive_signals();
    return (g_last_exit_status);
}
