/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:33:18 by shiori            #+#    #+#             */
/*   Updated: 2025/03/20 23:48:17 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	create_temp_file(void)
{
	int	tmp_fd;

	tmp_fd = open("/tmp/minishell_heredoc", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (tmp_fd);
}

static int	write_heredoc_content(int tmp_fd, char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
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
		if (write(tmp_fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

static int	setup_heredoc_input(int tmp_fd)
{
	close(tmp_fd);
	tmp_fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (tmp_fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(tmp_fd);
		return (-1);
	}
	unlink("/tmp/minishell_heredoc");
	close(tmp_fd);
	return (0);
}

int	handle_heredocument(char *delimiter, t_cmd *cmd)
{
    
	int	tmp_fd;
	
	if (cmd->backup_stdin == -1)
	{
		cmd->backup_stdin = dup(STDIN_FILENO);
		if (cmd->backup_stdin == -1)
		{
			perror("dup");
			return (-1);
		}
	}
	tmp_fd = create_temp_file();
	if (tmp_fd == -1)
		return (-1);
	if (write_heredoc_content(tmp_fd, delimiter) == -1)
	{
		close(tmp_fd);
		return (-1);
	}
	if (setup_heredoc_input(tmp_fd) == -1)
		return (-1);
	return (0);
}