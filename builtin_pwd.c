/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 01:20:17 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_pwd(char **cmd, t_list *env)
{
	char	*buffer;
	size_t	size;

	(void)cmd;
	(void)env;

	size = 128;
	buffer = (char *)ft_calloc(size, sizeof(char));
	if (!buffer)
		return (perror_int("malloc", errno));
	while (getcwd(buffer, size * sizeof(char)) == NULL)
	{
		free(buffer);
		if (errno != ERANGE)
			return (perror_int("getcwd", errno));
		size *= 2;
		buffer = (char *)ft_calloc(size, sizeof(char));
		if (!buffer)
			return (perror_int("malloc", errno));
	}
	printf("%s\n", buffer);
	free(buffer);
	return (0);
}
