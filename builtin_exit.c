/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:54:17 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:29:05 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_numeric(const char *str);
static bool	is_in_long_range(long *result, const char *str);

t_valid	builtin_exit(char **cmd, t_list *env)
{
	long	result;

	(void)env;
	result = 0;
	if (cmd[1] && (!is_numeric(cmd[1]) || !is_in_long_range(&result, cmd[1])))
	{
		if (write(STDERR_FILENO, "minishell: exit: ", 17) < 0
			|| write(STDERR_FILENO, cmd[1], ft_strlen(cmd[1])) < 0
			|| write(STDERR_FILENO, ": numeric argument required\n", 28) < 0)
		{
			perror("write");
			return (ERROR);
		}
		g_last_exit_status = 2;
		exit(g_last_exit_status);
	}
	if (cmd[1] && cmd[2])
		return (error_msg("exit", "too many arguments", INVALID));
	g_last_exit_status = (result % 256 + 256) % 256;
	if (write(STDOUT_FILENO, "exit\n", 5) < 0)
	{
		perror("write");
		return (ERROR);
	}
	return (VALID);
}

static bool	is_numeric(const char *str)
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

static bool	is_in_long_range(long *result, const char *str)
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
