/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:54:17 by shiori            #+#    #+#             */
/*   Updated: 2025/03/21 11:49:43 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// extern int g_last_exit_status;

bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (false);
	while (ft_strchr(" \t\n\v\f\r", str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
		i++;
	while (str[i] && ft_strchr(" \t\n\v\f\r", str[i]))
		i++;
	if (str[i])
		return (false);
	return (true);
}

bool	is_in_long_range(long *result, const char *str)
{
	int		sign;

	while (str && *str && ft_strchr(" \t\n\v\f\r", *str))
		str++;
	sign = 1;
	if (str && (*str == '-' || *str == '+'))
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (str && ('0' <= *str && *str <= '9'))
	{
		if (*result < LONG_MIN / 10 || LONG_MAX / 10 < *result)
			return (false);
		*result *= 10;
		if (*result < LONG_MIN + (*str - '0')
			|| LONG_MAX - (*str - '0') < *result)
			return (false);
		*result += sign * (*(str++) - '0');
	}
	return (true);
}

int	builtin_exit(char **cmd, t_list *env)
{
	long	result;

	(void)env;
	result = 0;
	if (cmd[1] && (!is_numeric(cmd[1]) || !is_in_long_range(&result, cmd[1])))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		g_last_exit_status = 2;  // Linuxでは終了コード2
		exit(g_last_exit_status);
	}
	if (cmd[1] && cmd[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_last_exit_status = 1;
		return (-1);
	}
	g_last_exit_status = (result % 256 + 256) % 256;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (0);
}
