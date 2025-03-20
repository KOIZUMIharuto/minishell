/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:16:44 by shiori            #+#    #+#             */
/*   Updated: 2025/03/21 01:01:50 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int restore_redirection(t_cmd *cmd)
{
    if (cmd->backup_stdin != -1)
    {
        
        if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
        {
            perror("dup2");
            return -1;
        }
        close(cmd->backup_stdin);
        cmd->backup_stdin = -1;
    }
    if (cmd->backup_stdout != -1)
    {
        if (dup2(cmd->backup_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            return -1;
        }
        close(cmd->backup_stdout);
        cmd->backup_stdout = -1;
    }
    return 0;
}

int    handle_ambiguous_rdrct(t_rdrct *rdrct)
{
    (void)rdrct;
    ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("$", STDERR_FILENO);
    ft_putstr_fd(rdrct->token, STDERR_FILENO);
    ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
    return (-1);
}
