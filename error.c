/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:58:22 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 23:48:01 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	error_msg(char *cmd, char *msg, t_valid is_valid)
{
	if (!print_msg("minishell: ", STDERR_FILENO)
		|| !print_msg(cmd, STDERR_FILENO)
		|| !print_msg(": ", STDERR_FILENO)
		|| !print_msg(msg, STDERR_FILENO)
		|| !print_msg("\n", STDERR_FILENO))
	{
		g_last_exit_status = -1;
		return (CRITICAL_ERROR);
	}
	return (is_valid);
}

int	perror_int(char *msg)
{
	perror(msg);
	g_last_exit_status = -1;
	return (1);
}

bool	perror_bool(char *msg)
{
	perror(msg);
	g_last_exit_status = -1;
	return (false);
}

void	*perror_ptr(char *msg)
{
	perror(msg);
	g_last_exit_status = -1;
	return (NULL);
}
