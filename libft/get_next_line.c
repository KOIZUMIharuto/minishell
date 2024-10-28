/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:52:07 by shiori            #+#    #+#             */
/*   Updated: 2024/07/14 15:58:53 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_line(char **string)
{
	char	*line;
	char	*new_string;
	char	*newline_pos;

	newline_pos = ft_strchr_for_gnl(*string, '\n');
	if (newline_pos)
	{
		new_string = ft_strdup_for_gnl(newline_pos + 1);
		*(newline_pos + 1) = '\0';
		line = ft_strdup_for_gnl(*string);
		free(*string);
		*string = new_string;
	}
	else
	{
		line = *string;
		*string = NULL;
	}
	return (line);
}

static void	ft_strdel(char **str)
{
	if (!str || !*str)
		return ;
	free(*str);
	*str = NULL;
}

ssize_t	read_from_fd(int fd, char **string)
{
	ssize_t	read_bytes;
	char	*buffer;
	char	*tmp;

	buffer = (char *)malloc((size_t)BUFFER_SIZE + (size_t)1);
	if (!buffer)
		return (-1);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		tmp = ft_strjoin_for_gnl(*string, buffer);
		if (!tmp)
		{
			free(buffer);
			return (-1);
		}
		ft_strdel(string);
		*string = tmp;
		if (ft_strchr_for_gnl(buffer, '\n'))
			break ;
		read_bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (read_bytes);
}

char	*get_next_line(int fd)
{
	static char	*string;
	ssize_t		read_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_bytes = read_from_fd(fd, &string);
	if (read_bytes < 0)
	{
		ft_strdel(&string);
		return (NULL);
	}
	if (read_bytes == 0 && ft_strlen_s_for_gnl(string) == 0)
	{
		ft_strdel(&string);
		return (NULL);
	}
	return (get_line(&string));
}
