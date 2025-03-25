/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:05:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/25 23:11:34 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int setup_heredoc_pipe(int pipe_fds[2], t_cmd *cmd)
{
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (cmd->backup_stdin == -1)
    {
        cmd->backup_stdin = dup(STDIN_FILENO);
        if (cmd->backup_stdin == -1)
        {
            perror("dup");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            return (-1);
        }
    }
    else
    {
        dup2(cmd->backup_stdin, STDIN_FILENO);
    }
    return (0);
}

static void	process_heredoc_child(int pipe_fds[2], char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	close(pipe_fds[0]);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || (ft_strncmp(line, delimiter, delimiter_len) == 0
				            && ft_strlen(line) - 1 == delimiter_len))
		{
			free(line);
			break ;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fds[1]);
	exit(0);
}

static int	setup_parent_process(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{
	waitpid(pid, NULL, 0);
	close(pipe_fds[1]);
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
		close(pipe_fds[0]);
		return (-1);
	}
	close(pipe_fds[0]);
	return (0);
}

int	handle_heredocument(char *delimiter, t_cmd *cmd)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (setup_heredoc_pipe(pipe_fds, cmd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	if (pid == 0)
		process_heredoc_child(pipe_fds, delimiter);
	return (setup_parent_process(pipe_fds, cmd, pid));
}