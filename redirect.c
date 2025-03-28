/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:51:26 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// static int	backup_io(t_cmd *cmd)
// {
// 	if (cmd->backup_stdin == -1)
// 	{
// 		cmd->backup_stdin = dup(STDIN_FILENO);
// 		if (cmd->backup_stdin == -1)
// 			return (perror_int("dup"));
// 	}
// 	if (cmd->backup_stdout == -1)
// 	{
// 		cmd->backup_stdout = dup(STDOUT_FILENO);
// 		if (cmd->backup_stdout == -1)
// 		{
// 			(void)perror_int("dup");
// 			close(cmd->backup_stdin);
// 			cmd->backup_stdin = -1;
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

static int	handle_input_rdrct(t_cmd *cmd, t_rdrct *rdrct, bool is_last)
{
	cmd->infile_fd = open(rdrct->file[0], O_RDONLY);
	if (cmd->infile_fd == -1)
		return (error_msg(rdrct->file[0], strerror(errno), INVALID));
	if (is_last)
		dup2(cmd->infile_fd, STDIN_FILENO);
	close(cmd->infile_fd);
	cmd->infile_fd = -1;
	return (0);
}

static int	handle_output_rdrct(t_cmd *cmd, t_rdrct *rdrct)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (rdrct->type == OVERWRITE_RDRCT)
		flags |= O_TRUNC;
	else if (rdrct->type == APPEND_RDRCT)
		flags |= O_APPEND;
	cmd->outfile_fd = open(rdrct->file[0], flags, 0644);
	if (cmd->outfile_fd == -1)
		return (error_msg(rdrct->file[0], strerror(errno), INVALID));
	dup2(cmd->outfile_fd, STDOUT_FILENO);
	close(cmd->outfile_fd);
	cmd->outfile_fd = -1;
	return (0);
}

bool	is_last_input(t_rdrct **redirect, int i)
{
	bool	is_last;

	is_last = true;
	while (redirect[++i])
	{
		if (redirect[i]->type == HEREDOCUMENT
			|| redirect[i]->type == INPUT_RDRCT)
		{
			is_last = false;
			break ;
		}
	}
	return (is_last);
}

int	handle_redirection(t_cmd *cmd, t_list *env)
{
	int		j;
	t_rdrct	*redirect;

	(void)env;
	j = 0;
	while (cmd->rdrcts[j])
	{
		redirect = cmd->rdrcts[j];
		if (redirect->type != HEREDOCUMENT
			&& (!redirect->file[0] || redirect->file[1]))
			return (error_msg(redirect->token, "ambiguous redirect", INVALID));
		if ((redirect->type == INPUT_RDRCT && handle_input_rdrct(cmd, redirect, is_last_input(cmd->rdrcts, j)))
			|| ((redirect->type == OVERWRITE_RDRCT || redirect->type == APPEND_RDRCT)
				&& handle_output_rdrct(cmd, redirect)))
			return (1);
		j++;
	}
	return (0);
}

int restore_redirection(t_cmd *cmd)
{
	if (cmd->backup_stdin != -1)
	{
		if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
			return (perror_int("dup2"));
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
	}
	if (cmd->backup_stdout != -1)
	{
		if (dup2(cmd->backup_stdout, STDOUT_FILENO) == -1)
			return (perror_int("dup2"));
		close(cmd->backup_stdout);
		cmd->backup_stdout = -1;
	}
	return (0);
}
