/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:58:22 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 13:53:56 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

int	perror_int(char *msg, int errnum)
{
	my_perror(msg, errnum);
	return (1);
}

bool	perror_bool(char *msg, int errnum)
{
	my_perror(msg, errnum);
	return (false);
}

void	*perror_ptr(char *msg, int errnum)
{
	my_perror(msg, errnum);
	return (NULL);
}

void	my_perror(char *msg, int errnum)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
