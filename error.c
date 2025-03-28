/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:58:22 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/28 19:16:37 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	error_msg(char *command, char *msg, t_valid is_valid)
{
	if (!print_msg("minishell: ", STDERR_FILENO)
		|| !print_msg(command, STDERR_FILENO)
		|| !print_msg(": ", STDERR_FILENO)
		|| !print_msg(msg, STDERR_FILENO)
		|| !print_msg("\n", STDERR_FILENO))
	{
		g_last_exit_status = -1;
		return (ERROR);
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
