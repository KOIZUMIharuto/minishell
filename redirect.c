/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 17:15:30 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	handle_ambiguous_redirect(t_rdrct *rdrct, t_cmd *cmd)
{
	ft_putstr_fd(rdrct->token, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	restore_redirection(cmd);
	return (-1);
}

static int	handle_input_redirect(t_cmd *cmd, t_rdrct *rdrct)
{
	if (rdrct->file[1] != NULL)
		return (handle_ambiguous_redirect(rdrct, cmd));
	cmd->backup_stdin = dup(STDIN_FILENO);
	if (cmd->backup_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	cmd->infile_fd = open(rdrct->file[0], O_RDONLY);
	if (cmd->infile_fd == -1)
	{
		ft_putstr_fd(rdrct->token, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		restore_redirection(cmd);
		return (-1);
	}
	dup2(cmd->infile_fd, STDIN_FILENO);
	close(cmd->infile_fd);
	return (0);
}

static int	handle_output_redirect(t_cmd *cmd, t_rdrct *rdrct)
{
	int	flags;

	if (rdrct->file[1] != NULL)
		return (handle_ambiguous_redirect(rdrct, cmd));
	cmd->backup_stdout = dup(STDOUT_FILENO);
	if (cmd->backup_stdout == -1)
	{
		perror("dup");
		return (-1);
	}
	flags = O_WRONLY | O_CREAT;
	if (rdrct->type == APPEND_RDRCT)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	cmd->outfile_fd = open(rdrct->file[0], flags, 0644);
	if (cmd->outfile_fd == -1)
	{
		ft_putstr_fd(rdrct->token, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		restore_redirection(cmd);
		return (-1);
	}
	dup2(cmd->outfile_fd, STDOUT_FILENO);
	close(cmd->outfile_fd);
	return (0);
}

int	handle_redirection(t_cmd *cmd)
{
	int	j;

	j = 0;
	while (cmd->input_rdrct[j])
	{
		if (cmd->input_rdrct[j]->type == INPUT_RDRCT)
		{
			if (handle_input_redirect(cmd, cmd->input_rdrct[j]) == -1)
				return (-1);
		}
		j++;
	}
	j = 0;
	while (cmd->output_rdrct[j])
	{
		if (handle_output_redirect(cmd, cmd->output_rdrct[j]) == -1)
			return (-1);
		j++;
	}
	return (0);
}


