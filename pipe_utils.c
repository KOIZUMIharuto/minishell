/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 02:59:56 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int process_heredocs(t_cmd *cmd)
{
    int j;
    
    j = 0;
    while (cmd->input_rdrct[j])
    {
        if (cmd->input_rdrct[j]->type == HEREDOCUMENT)
        {
            if (handle_heredocument(cmd->input_rdrct[j]->file[0], cmd) == -1)
                return -1;
        }
        j++;
    }
    return 0;
}

int setup_pipe(t_pipe_info *pipe_info, bool has_next)
{
    pipe_info->has_next = has_next;
    
    if (has_next)
    {
        if (pipe(pipe_info->pipe_fds) == -1)
        {
            perror("pipe");
            return -1;
        }
    }
    return 0;
}

void manage_parent_pipes(t_pipe_info *pipe_info)
{
    if (pipe_info->prev_fd != -1)
        close(pipe_info->prev_fd);
        
    if (pipe_info->has_next)
    {
        close(pipe_info->pipe_fds[1]);
        pipe_info->prev_fd = pipe_info->pipe_fds[0];
    }
}

