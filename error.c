/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:58:22 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/25 17:14:57 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	error_msg(char *command, char *msg)
{
	if (!print_msg("minishell: ", STDERR_FILENO)
		|| !print_msg(command, STDERR_FILENO)
		|| !print_msg(": ", STDERR_FILENO)
		|| !print_msg(msg, STDERR_FILENO)
		|| !print_msg("\n", STDERR_FILENO))
	{
		g_last_exit_status = EXIT_FAILURE;
	}
	return (1);
}

int	perror_int(char *msg)
{
	perror(msg);
	return (1);
}

bool	perror_bool(char *msg)
{
	perror(msg);
	return (false);
}

void	*perror_ptr(char *msg)
{
	perror(msg);
	return (NULL);
}
