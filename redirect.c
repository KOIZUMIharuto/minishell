/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:26:06 by shiori           ###   ########.fr       */
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

static t_valid handle_input_rdrct(t_cmd *cmd, t_rdrct *rdrct, bool is_last)
{
	cmd->infile_fd =open(rdrct->file[0], O_RDONLY);
	if (cmd->infile_fd == -1)
	{
        error_msg(rdrct->file[0], strerror(errno));
        return (CRITICAL_ERROR);
    }
    if (is_last)
    {
        if(dup2(cmd->infile_fd, STDIN_FILENO)==-1)
        {
            perror("dup2");
            close(cmd->infile_fd);
            return (CRITICAL_ERROR);
        }
    }
    close(cmd->infile_fd);
    cmd->infile_fd = -1;
    return (VALID);
}

static t_valid handle_output_rdrct(t_cmd *cmd, t_rdrct *rdrct)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (rdrct->type == OVERWRITE_RDRCT)
		flags |= O_TRUNC;
	else if (rdrct->type == APPEND_RDRCT)
		flags |= O_APPEND;
	cmd->outfile_fd = open(rdrct->file[0], flags, 0644);
	if (cmd->outfile_fd == -1)
    {
		error_msg(rdrct->file[0], strerror(errno));
        return (CRITICAL_ERROR);
    }
	if(dup2(cmd->outfile_fd, STDOUT_FILENO)==-1)
	{
		perror("dup2");
		close(cmd->outfile_fd);
		return (CRITICAL_ERROR);
	}
	close(cmd->outfile_fd);
	cmd->outfile_fd = -1;
	return (VALID);
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

t_valid handle_redirection(t_cmd *cmd, t_list *env)
{
	int		j;
	t_rdrct	*redirect;

	(void)env;
	// if (backup_io(cmd))
	// 	return (1);
	// if (cmd->backup_stdin != -1)
	// 	close(cmd->backup_stdin);
	j = 0;
	while (cmd->rdrcts[j])
	{
		redirect = cmd->rdrcts[j];
		if (redirect->type != HEREDOCUMENT
			&& (!redirect->file[0] || redirect->file[1]))
			return (error_msg(redirect->token, "ambiguous redirect"));
		// if ((redirect->type == HEREDOCUMENT && process_heredocs(cmd, redirect, env) == -1)
		// 	|| (redirect->type == INPUT_RDRCT && handle_input_rdrct(cmd, redirect))
		if ((redirect->type == INPUT_RDRCT && handle_input_rdrct(cmd, redirect, is_last_input(cmd->rdrcts, j)==CRITICAL_ERROR))
			|| ((redirect->type == OVERWRITE_RDRCT || redirect->type == APPEND_RDRCT)
				&& handle_output_rdrct(cmd, redirect))==CRITICAL_ERROR)
			return (CRITICAL_ERROR);
		j++;
	}
	return (VALID);
}

t_valid restore_redirection(t_cmd *cmd)
{
	if (cmd->backup_stdin != -1)
	{
		if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
        {
            perror_int("dup2");
		    close(cmd->backup_stdin);
            return (CRITICAL_ERROR);
        }
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
	}
	if (cmd->backup_stdout != -1)
	{
		if (dup2(cmd->backup_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(cmd->backup_stdout);
            return (CRITICAL_ERROR);
        }
		close(cmd->backup_stdout);
		cmd->backup_stdout = -1;
	}
	return (VALID);
}
