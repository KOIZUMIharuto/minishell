/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:30:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 13:37:34 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid setup_pipe(t_pipe_info *pipe_info, bool has_next)
{
    pipe_info->has_next = has_next;
    
    if (has_next)
    {
        if (pipe(pipe_info->pipe_fds) == -1)
        {
            perror("pipe");
            return (CRITICAL_ERROR);
        }
    }
    return (VALID);
}

void manage_pipes(t_pipe_info *pipe_info)
{
	close_wrapper(&(pipe_info->prev_fd));
    if (pipe_info->has_next)
    {
        close_wrapper(&(pipe_info->pipe_fds[1]));
        pipe_info->prev_fd = pipe_info->pipe_fds[0];
    }
}

t_valid handle_pipe_input(t_pipe_info *pipe_info)
{
    if (pipe_info->prev_fd != -1)
    {
        if(dup2(pipe_info->prev_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close_wrapper(&(pipe_info->prev_fd));
            return (CRITICAL_ERROR);
        }
        close_wrapper(&(pipe_info->prev_fd));
    }
    return (VALID);
}

t_valid handle_pipe_output(t_pipe_info *pipe_info)
{
    if (pipe_info->has_next)
    {
        if(dup2(pipe_info->pipe_fds[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
			close_wrapper(&(pipe_info->pipe_fds[1]));
            close_wrapper(&(pipe_info->pipe_fds[0]));
            exit(CRITICAL_ERROR);
        }
        close_wrapper(&(pipe_info->pipe_fds[1]));
        close_wrapper(&(pipe_info->pipe_fds[0]));
    }
    return (VALID);
}
