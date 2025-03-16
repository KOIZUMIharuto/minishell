/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:04 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 20:43:29 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <minishell.h>


int execute_builtin(t_cmd *cmd, int (*func)(char **, t_list *), t_list *env)
{
    int result;

    // ヒアドキュメント処理
    for (int j = 0; cmd->input_rdrct[j]; j++)
    {
        if (cmd->input_rdrct[j]->type == HEREDOCUMENT)
        {
            if (handle_heredocument(cmd->input_rdrct[j]->file[0], cmd) == -1)
                return EXIT_FAILURE;
        }
    }

    // リダイレクトの処理
    if (handle_redirection(cmd) == -1)
        return EXIT_FAILURE;

    // コマンドを実行
    result = func(cmd->cmd, env);

    // リダイレクトを元に戻す
    restore_redirection(cmd);

    g_last_exit_status = result;
    return result;
}
