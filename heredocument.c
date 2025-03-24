/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:33:18 by shiori            #+#    #+#             */
/*   Updated: 2025/03/24 11:15:06 by hkoizumi         ###   ########.fr       */
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


static int	write_expand_env(int tmp_fd, char *line, t_list *env_list)
{
	t_env	*env;
	int		key_len;

	while (*line)
	{
		if (*line == '$')
		{
			if (line[1] == '?')
			{
				ft_putnbr_fd(g_last_exit_status, tmp_fd);
				line += 2;
				continue ;
			}
			else if (ft_isalpha(line[1]) || line[1] == '_')
			{
				while (env_list)
				{
					env = (t_env *)env_list->content;
					key_len = ft_strlen(env->key);
					if (ft_strncmp(env->key, &(line[1]), key_len) == 0
						&& !(ft_isalnum(line[key_len + 1]) || line[key_len + 1] == '_'))
					{
						if (write(tmp_fd, env->value, ft_strlen(env->value)) == -1)
							return (-1);
						line += key_len + 1;
						break ;
					}
					env_list = env_list->next;
				}
				if (env_list)
					continue ;
			}
		}
		if (write(tmp_fd, line++, 1) == -1)
			return (-1);
	}
	return (0);
}

static int	write_heredoc_content(int tmp_fd, t_rdrct *rdrct, t_list *env)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(rdrct->file[0]);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || (ft_strncmp(line, rdrct->file[0], delimiter_len) == 0
				&& ft_strlen(line) - 1 == delimiter_len))
		{
			free(line);
			break ;
		}
		if (rdrct->is_quoted && write(tmp_fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (-1);
		}
		else if (!rdrct->is_quoted && write_expand_env(tmp_fd, line, env) == -1)
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

int	handle_heredocument(t_rdrct *redirect, t_cmd *cmd, t_list *env)
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
	if (write_heredoc_content(tmp_fd, redirect, env) == -1)
	{
		close(tmp_fd);
		return (-1);
	}
	if (setup_heredoc_input(tmp_fd) == -1)
		return (-1);
	return (0);
}