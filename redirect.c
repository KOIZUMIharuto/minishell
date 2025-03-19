/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 23:34:00 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	backup_io(t_cmd *cmd)
{
	cmd->backup_stdin = dup(STDIN_FILENO);
	if (cmd->backup_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	cmd->backup_stdout = dup(STDOUT_FILENO);
	if (cmd->backup_stdout == -1)
	{
		perror("dup");
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
		return (-1);
	}
	return (0);
}

static int	handle_input_rdrct(t_cmd *cmd, t_rdrct *rdrct)
{
	if (rdrct->file[1] != NULL)
		return (handle_ambiguous_rdrct(rdrct));
	
	cmd->infile_fd = open(rdrct->file[0], O_RDONLY);
	if (cmd->infile_fd == -1)
	{
		ft_putstr_fd(rdrct->file[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	dup2(cmd->infile_fd, STDIN_FILENO);
	close(cmd->infile_fd);
	return (0);
}

static int	handle_output_rdrct(t_cmd *cmd, t_rdrct *rdrct)
{
	int	flags;

	if (rdrct->file[1] != NULL)
		return (handle_ambiguous_rdrct(rdrct));
	
	flags = O_WRONLY | O_CREAT;
	if (rdrct->type == OVERWRITE_RDRCT)
		flags |= O_TRUNC;
	else if (rdrct->type == APPEND_RDRCT)
		flags |= O_APPEND;
	cmd->outfile_fd = open(rdrct->file[0], flags, 0644);
	if (cmd->outfile_fd == -1)
	{
		ft_putstr_fd(rdrct->file[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	dup2(cmd->outfile_fd, STDOUT_FILENO);
	close(cmd->outfile_fd);
	return (0);
}

int	handle_redirection(t_cmd *cmd)
{
	int	j;

	if (backup_io(cmd) == -1)
		return (-1);
		
	j = 0;
	while (cmd->input_rdrct[j])
	{
		if (cmd->input_rdrct[j]->type == INPUT_RDRCT)
		{
			if (handle_input_rdrct(cmd, cmd->input_rdrct[j]) == -1)
				return (-1);
		}
		j++;
	}
	j = 0;
	while (cmd->output_rdrct[j])
	{
		if (cmd->output_rdrct[j]->type == OVERWRITE_RDRCT
			|| cmd->output_rdrct[j]->type == APPEND_RDRCT)
		{
			if (handle_output_rdrct(cmd, cmd->output_rdrct[j]) == -1)
				return (-1);
		}
		j++;
	}
	return (0);
}


