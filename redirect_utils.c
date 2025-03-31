/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:44:54 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 13:47:06 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	restore_redirection(t_cmd *cmd)
{
	if (cmd->backup_stdin != -1)
	{
		if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close_wrapper(&(cmd->backup_stdin));
			return (CRITICAL_ERROR);
		}
		close_wrapper(&(cmd->backup_stdin));
	}
	if (cmd->backup_stdout != -1)
	{
		if (dup2(cmd->backup_stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close_wrapper(&(cmd->backup_stdout));
			return (CRITICAL_ERROR);
		}
		close_wrapper(&(cmd->backup_stdout));
	}
	return (VALID);
}
