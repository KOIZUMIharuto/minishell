/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:28 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 21:24:39 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	print_token(char **token, bool newline);

t_valid	builtin_echo(char **cmd, t_list *env)
{
	int		i;
	int		j;
	bool	newline;

	(void)env;
	i = 1;
	newline = true;
	while (cmd[i] && ft_strncmp(cmd[i], "-n", 2) == 0)
	{
		j = 2;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j])
			break ;
		newline = false;
		i++;
	}
	if (print_token(&(cmd[i]), newline))
		return (CRITICAL_ERROR);
	return (VALID);
}

static int	print_token(char **token, bool newline)
{
	while (*token)
	{
		if (write(STDOUT_FILENO, *token, ft_strlen(*token)) < 0)
			return (perror_int("write"));
		if (*(token + 1) && write(STDOUT_FILENO, " ", 1) < 0)
			return (perror_int("write"));
		token++;
	}
	if (newline && write(STDOUT_FILENO, "\n", 1) < 0)
		return (perror_int("write"));
	return (0);
}
