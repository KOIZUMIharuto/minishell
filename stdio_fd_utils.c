/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdio_fd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:20:15 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 20:21:03 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// リダイレクトを元に戻す関数を追加
int restore_rdrction(t_cmd *cmd)
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