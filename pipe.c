/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 01:16:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 23:34:24 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <minishell.h>

int execute_pipeline(t_cmd **cmds, t_builtin *builtins, t_list *env)
{
    // パイプラインが存在するかチェック
    bool is_pipeline = false;
    int cmd_count = 0;
    while (cmds[cmd_count])
        cmd_count++;
    if (cmd_count > 1)
        is_pipeline = true;

    // 単一コマンドかつビルトインの場合、親プロセスで直接実行
    if (!is_pipeline)
    {
        int is_builtin_index = get_builtin_index(builtins, cmds[0]->cmd[0]);
        if (is_builtin_index >= 0)
        {
            setup_builtin_signals();
            
            // ヒアドキュメントとリダイレクト処理
            for (int j = 0; cmds[0]->input_rdrct[j]; j++)
            {
                if (cmds[0]->input_rdrct[j]->type == HEREDOCUMENT)
                {
                    if (handle_heredocument(cmds[0]->input_rdrct[j]->file[0], cmds[0]) == -1)
                        return EXIT_FAILURE;
                }
            }
            
            // リダイレクト処理
            if (handle_redirection(cmds[0]) == -1)
                return EXIT_FAILURE;
                
            // ビルトイン実行
            int result = builtins[is_builtin_index].func(cmds[0]->cmd, env);
            
            // リダイレクトを元に戻す
            restore_redirection(cmds[0]);
            
            // exit コマンドの特別処理
            if (ft_strcmp(cmds[0]->cmd[0], "exit") == 0)
            {
                g_last_exit_status = result;
                return -42;  // シェル終了を示す特別な値
            }
            
            g_last_exit_status = result;
            setup_interactive_signals();
            return result;
        }
    }

    // ここから複数コマンドのパイプライン処理、または単一の外部コマンド処理
    int pipe_fds[2];
    int prev_fd = -1;
    int status;
    int i = 0;
    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
        return EXIT_FAILURE;

    // 以降は前回提案した内容と同じ（すべてのコマンドを子プロセスで実行）
    while (cmds[i])
    {
        if (cmds[i + 1])
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                free(pids);
                return (EXIT_FAILURE);
            }
        }

        int is_builtin_index = get_builtin_index(builtins, cmds[i]->cmd[0]);
        
        setup_exec_signals();
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(pids);
            return EXIT_FAILURE;
        }

        if (pid == 0)
        {
            // 子プロセスの処理（前回と同様）
            setup_child_signals();
            
            // ヒアドキュメント処理
            for (int j = 0; cmds[i]->input_rdrct[j]; j++)
            {
                if (cmds[i]->input_rdrct[j]->type == HEREDOCUMENT)
                {
                    if (handle_heredocument(cmds[i]->input_rdrct[j]->file[0], cmds[i]) == -1)
                        exit(EXIT_FAILURE);
                }
            }

            // リダイレクト処理
            if (handle_redirection(cmds[i]) == -1)
                exit(EXIT_FAILURE);

            // パイプの設定
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

            // ビルトインまたは外部コマンド実行
            if (is_builtin_index >= 0)
            {
                int result = builtins[is_builtin_index].func(cmds[i]->cmd, env);
                exit(result);
            }
            else
            {
                execute_cmd(cmds[i]->cmd, env);
            }
        }
        else
        {
            // 親プロセスの処理（前回と同様）
            pids[i] = pid;

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
        i++;
    }

    // 子プロセスの待機処理
    for (i = 0; i < cmd_count; i++)
    {
        waitpid(pids[i], &status, 0);
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                g_last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                g_last_exit_status = WTERMSIG(status) + 128;
        }
    }

    free(pids);
    setup_interactive_signals();
    return g_last_exit_status;
}