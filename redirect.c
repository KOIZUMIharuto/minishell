/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 23:24:05 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int		backup_io(t_cmd *cmd);
static t_valid	handle_input_rdrct(t_cmd *cmd, t_rdrct *rdrct, int i);
static t_valid	handle_output_rdrct(t_cmd *cmd, t_rdrct *rdrct);
static bool		is_last_input_rdrct(t_rdrct **redirect, int i);

t_valid	handle_redirection(t_cmd *cmd, t_list *env)
{
	int		i;
	t_rdrct	*rdrct;
	t_valid	is_valid;

	(void)env;
	if (backup_io(cmd))
		return (CRITICAL_ERROR);
	i = 0;
	is_valid = VALID;
	while (cmd->rdrcts[i])
	{
		rdrct = cmd->rdrcts[i];
		if (rdrct->type != HEREDOCUMENT
			&& (!rdrct->file[0] || rdrct->file[1]))
			return (error_msg(rdrct->token, "ambiguous redirect", INVALID));
		if (rdrct->type == INPUT_RDRCT)
			is_valid = handle_input_rdrct(cmd, rdrct, i);
		else if (rdrct->type == OVERWRITE_RDRCT || rdrct->type == APPEND_RDRCT)
			is_valid = handle_output_rdrct(cmd, rdrct);
		if (is_valid != VALID)
			return (is_valid);
		i++;
	}
	return (is_valid);
}

static int	backup_io(t_cmd *cmd)
{
	if (cmd->backup_stdin == -1)
	{
		cmd->backup_stdin = dup(STDIN_FILENO);
		if (cmd->backup_stdin == -1)
			return (perror_int("dup"));
	}
	if (cmd->backup_stdout == -1)
	{
		cmd->backup_stdout = dup(STDOUT_FILENO);
		if (cmd->backup_stdout == -1)
		{
			(void)perror_int("dup");
			close_wrapper(&(cmd->backup_stdin));
			return (1);
		}
	}
	return (0);
}

static t_valid	handle_input_rdrct(t_cmd *cmd, t_rdrct *rdrct, int i)
{
	bool	is_last;

	is_last = is_last_input_rdrct(cmd->rdrcts, i);
	cmd->infile_fd = open(rdrct->file[0], O_RDONLY);
	if (cmd->infile_fd == -1)
		return (error_msg(rdrct->file[0], strerror(errno), INVALID));
	if (is_last)
	{
		if (dup2(cmd->infile_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close_wrapper(&(cmd->infile_fd));
			return (CRITICAL_ERROR);
		}
	}
	close_wrapper(&(cmd->infile_fd));
	return (VALID);
}

static t_valid	handle_output_rdrct(t_cmd *cmd, t_rdrct *rdrct)
{
	int		flags;
	bool	is_success;

	flags = O_WRONLY | O_CREAT;
	if (rdrct->type == OVERWRITE_RDRCT)
		flags |= O_TRUNC;
	else if (rdrct->type == APPEND_RDRCT)
		flags |= O_APPEND;
	cmd->outfile_fd = open(rdrct->file[0], flags, 0644);
	is_success = true;
	if (cmd->outfile_fd == -1)
	{
		if (errno == EACCES || errno == ENOTDIR || errno == EISDIR)
			return (error_msg(rdrct->file[0], strerror(errno), INVALID));
		else
			is_success = perror_bool("open");
	}
	if (is_success && dup2(cmd->outfile_fd, STDOUT_FILENO) == -1)
		is_success = perror_bool("dup2");
	close_wrapper(&(cmd->outfile_fd));
	if (!is_success)
		return (CRITICAL_ERROR);
	return (VALID);
}

static bool	is_last_input_rdrct(t_rdrct **redirect, int i)
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
