/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:28 by shiori            #+#    #+#             */
/*   Updated: 2025/03/24 20:20:14 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	print_token(char **token, bool newline);

int	builtin_echo(char **cmd, t_list *env)
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
	print_token(&(cmd[i]), newline);
	return (0);
}

static void	print_token(char **token, bool newline)
{
	while (*token)
	{
		ft_putstr_fd(*token, STDOUT_FILENO);
		if (*(token + 1))
			ft_putchar_fd(' ', STDOUT_FILENO);
		token++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
