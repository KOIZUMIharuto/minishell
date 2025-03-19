/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdrct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:05:03 by shiori            #+#    #+#             */
/*   Updated: 2025/03/19 12:10:44 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int handle_rdrction(t_cmd *cmd)
{
    // 入力リダイレクト処理
    for (int j = 0; cmd->input_rdrct[j]; j++)
    {
        if (cmd->input_rdrct[j]->type == INPUT_REDIRECT)
        {
            cmd->backup_stdin = dup(STDIN_FILENO);  // 毎回バックアップを取る
            if (cmd->backup_stdin == -1)
            {
                perror("dup");
                return -1;
            }

            cmd->infile_fd = open(cmd->input_rdrct[j]->file[0], O_RDONLY);
            if (cmd->infile_fd == -1)
            {
                perror("open");
                return -1;
            }
            dup2(cmd->infile_fd, STDIN_FILENO);
            close(cmd->infile_fd);
        }
    }

    // 出力リダイレクト処理
    for (int j = 0; cmd->output_rdrct[j]; j++)
    {
        cmd->backup_stdout = dup(STDOUT_FILENO);  // 毎回バックアップを取る
        if (cmd->backup_stdout == -1)
        {
            perror("dup");
            return -1;
        }

        int flags = O_WRONLY | O_CREAT;
        if (cmd->output_rdrct[j]->type == APPEND_REDIRECT)
            flags |= O_APPEND;
        else  // OVERWRITE_REDIRECT
            flags |= O_TRUNC;

        cmd->outfile_fd = open(cmd->output_rdrct[j]->file[0], flags, 0644);
        if (cmd->outfile_fd == -1)
        {
            perror("open");
            return -1;
        }
        dup2(cmd->outfile_fd, STDOUT_FILENO);
        close(cmd->outfile_fd);
    }

    return 0;
}


