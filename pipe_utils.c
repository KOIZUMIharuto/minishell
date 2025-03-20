/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/20 11:16:16 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

void manage_pipes(t_pipe_info *pipe_info)
{
    if (pipe_info->prev_fd != -1)
        close(pipe_info->prev_fd);
        
    if (pipe_info->has_next)
    {
        close(pipe_info->pipe_fds[1]);
        pipe_info->prev_fd = pipe_info->pipe_fds[0];
    }
}

void handle_pipe_input(t_pipe_info *pipe_info)
{
    if (pipe_info->prev_fd != -1)
    {
        dup2(pipe_info->prev_fd, STDIN_FILENO);
        close(pipe_info->prev_fd);
    }
}

void handle_pipe_output(t_pipe_info *pipe_info)
{
    if (pipe_info->has_next)
    {
        dup2(pipe_info->pipe_fds[1], STDOUT_FILENO);
        close(pipe_info->pipe_fds[1]);
        close(pipe_info->pipe_fds[0]);
    }
}
