/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/17 14:04:04 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_pwd(char **cmd, t_list *env)
{
	char	*pwd;

	(void)cmd;
	(void)env;
	pwd = get_pwd();
	if (!pwd)
		return (1);
	ft_printf("%s\n", pwd);
	free(pwd);
	return (0);
}

char	*get_pwd(void)
{
	char	*buffer;
	size_t	size;

	size = 128;
	buffer = (char *)ft_calloc(size, sizeof(char));
	if (!buffer)
		return ((char *)perror_ptr("malloc", errno));
	while (getcwd(buffer, size * sizeof(char)) == NULL)
	{
		free(buffer);
		if (errno != ERANGE)
			return ((char *)perror_ptr("getcwd", errno));
		size *= 2;
		buffer = (char *)ft_calloc(size, sizeof(char));
		if (!buffer)
			return ((char *)perror_ptr("malloc", errno));
	}
	return (buffer);
}
