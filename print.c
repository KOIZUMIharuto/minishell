/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:14:24 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/25 12:17:13 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	print_msg(char *msg, int fd)
{
	if (write(fd, msg, ft_strlen(msg)) == -1)
	{
		perror("write");
		return (false);
	}
	return (true);
}
