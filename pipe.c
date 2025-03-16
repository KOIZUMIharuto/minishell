/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:16:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 12:10:43 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <minishell.h>

int execute_pipeline(t_cmd **cmds, char **env)
{
    int pipe_fds[2];
    int prev_fd = -1;
    int status;
    int i = 0;

    while (cmds[i])
    {
        // 次のコマンドがある場合のみパイプを作成
        if (cmds[i + 1])
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                return (EXIT_FAILURE);
            }
        }

        // ビルトインコマンドのチェック
        int is_builtin_index = is_builtin_mark_index(cmds[i]->cmd[0]);
        if (is_builtin_index >= 0)
        {
            setup_builtin_signals();
            
            // 入力/出力のリダイレクト設定
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);  // 使用後のfdをクローズ
            }
            if (cmds[i + 1])
            {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);  // 使用後のfdをクローズ
            }

            // ビルトインコマンド実行
            int result = execute_builtin(cmds[i]->cmd, is_builtin_index);
            
            // exit コマンドの特別処理
            if (ft_strcmp(cmds[i]->cmd[0], "exit") == 0 && !cmds[i + 1])
            {
                g_last_exit_status = result;
                return -42;
            }

            // パイプのクリーンアップと次のコマンドへの受け渡し
            if (cmds[i + 1])
            {
                close(pipe_fds[1]);
            }
            if (prev_fd != -1)
            {
                close(prev_fd);
            }
            if (cmds[i + 1])
            {
                prev_fd = pipe_fds[0];
            }

            setup_interactive_signals();
        }
        else
        {
            // 外部コマンドの処理
            setup_exec_signals();
            int pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0)
            {
                // 子プロセスの処理
                setup_child_signals();

                // ヒアドキュメント処理（これは最初に行う必要がある）
                for (int j = 0; cmds[i]->input_rdrct[j]; j++)
                {
                    if (cmds[i]->input_rdrct[j]->type == HEREDOCUMENT)
                    {
                        if (handle_heredocument_with_delimiter(cmds[i]->input_rdrct[j]->file[0]) == -1)
                            exit(EXIT_FAILURE);
                    }
                }

                // 入力リダイレクト処理（ヒアドキュメント以外）
                for (int j = 0; cmds[i]->input_rdrct[j]; j++)
                {
                    if (cmds[i]->input_rdrct[j]->type == INPUT_RDRCT)
                    {
                        cmds[i]->infile_fd = open(cmds[i]->input_rdrct[j]->file[0], O_RDONLY);
                        if (cmds[i]->infile_fd == -1)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        dup2(cmds[i]->infile_fd, STDIN_FILENO);
                        close(cmds[i]->infile_fd);
                    }
                }

                // 出力リダイレクト処理
                for (int j = 0; cmds[i]->output_rdrct[j]; j++)
                {
                    int flags = O_WRONLY | O_CREAT;
                    if (cmds[i]->output_rdrct[j]->type == APPEND_RDRCT)
                        flags |= O_APPEND;
                    else  // OVERWRITE_RDRCT
                        flags |= O_TRUNC;

                    cmds[i]->outfile_fd = open(cmds[i]->output_rdrct[j]->file[0], flags, 0644);
                    if (cmds[i]->outfile_fd == -1)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    dup2(cmds[i]->outfile_fd, STDOUT_FILENO);
                    close(cmds[i]->outfile_fd);
                }

                // パイプの設定（リダイレクトの後に行う）
                if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }
                if (cmds[i + 1])
                {
                    dup2(pipe_fds[1], STDOUT_FILENO);
                    close(pipe_fds[1]);
                    close(pipe_fds[0]);
                }

                execute_command(cmds[i]->cmd, env);
            }
            else
            {
                // 親プロセスでのパイプ管理
                if (prev_fd != -1)
                {
                    close(prev_fd);
                }
                if (cmds[i + 1])
                {
                    close(pipe_fds[1]);
                    prev_fd = pipe_fds[0];
                }
            }
        }
        i++;
    }

    // すべての子プロセスの終了を待つ
    while (wait(&status) > 0)
        ;

    // 終了ステータス処理
    if (WIFEXITED(status))
        g_last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_last_exit_status = WTERMSIG(status) + 128;

    // 対話モード用のシグナル設定に戻す
    setup_interactive_signals();

    return g_last_exit_status;
}