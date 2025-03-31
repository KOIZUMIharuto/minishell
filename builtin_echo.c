/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:28 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 23:35:28 by hkoizumi         ###   ########.fr       */
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
		if (!print_msg(*token, STDOUT_FILENO)
			|| (*(token + 1) && !print_msg(" ", STDOUT_FILENO)))
			return (1);
		token++;
	}
	if (newline && !print_msg("\n", STDOUT_FILENO))
		return (1);
	return (0);
}
