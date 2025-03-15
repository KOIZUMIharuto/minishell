// pipe.c
#include <minishell.h>

// extern int g_last_exit_status;

// パイプ処理を行う関数
// hkoizumi: (t_cmd **, t_builtin *, t_list *)を受け取るようにしたい
int pipe_command(char **commands, t_builtin *builtins, t_list *env)
{
    int num_commands = 0;
    while (commands[num_commands] != NULL) {
        num_commands++;
    }

    int pipe_fds[2];  // 現在のパイプ用の配列
    int prev_fd = -1; // 前のプロセスの出力ファイルディスクリプタ
    int status;

    // 各コマンドに対してループ処理
    for (int i = 0; i < num_commands; i++) {
        // 次のコマンドがある場合のみパイプを作成
        if (i < num_commands - 1) {
            if (pipe(pipe_fds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        
        char **command_tokens = ft_split(commands[i], ' ');	// hkoizumi: t_cmdの時点で解析済みなので不要

		// hkoizumi: 此処から先はt_cmdでの引き渡しに合わせて修正が必要
        // 組み込みコマンドの判定
        int builtin_index = get_builtin_index(builtins, command_tokens[0]);
        if (builtin_index >= 0) {;
            // 親プロセス内で組み込みコマンドを実行
            if (prev_fd != -1) {
                // 前のパイプからの入力を標準入力にリダイレクト
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < num_commands - 1) {
                // 標準出力を次のパイプの書き込み側にリダイレクト
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            // 組み込みコマンドを実行
            int result = execute_builtin(command_tokens, builtins[builtin_index].func, env);

             // exit コマンドが単独で実行された場合（パイプなし）は特別な戻り値を返す
            if (ft_strcmp(command_tokens[0], "exit") == 0 && num_commands == 1) {
                if (result == 1) {
                    continue;  // または適切な処理で次のコマンドへ進む
                }
                // command_tokens のメモリを解放
                for (int j = 0; command_tokens[j]; j++) {
                    free(command_tokens[j]);
                }
                free(command_tokens);
                g_last_exit_status = result;
                return -42;  // exit コマンドが実行されたことを示す特別な値
            }
            // 組み込みコマンドの後は標準出力を元に戻す
            if (i < num_commands - 1) {
                close(pipe_fds[1]);
            }

            // パイプ処理
            if (prev_fd != -1) {
                close(prev_fd);
            }
            prev_fd = pipe_fds[0];

        } else {
            // 外部コマンドの場合は子プロセスで処理
            int pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) { // 子プロセス内の処理
                if(handle_heredocument(command_tokens) == -1)
                    exit(EXIT_FAILURE);

                if (prev_fd != -1) {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }

                if (i < num_commands - 1) {
                    dup2(pipe_fds[1], STDOUT_FILENO);
                    close(pipe_fds[0]);
                    close(pipe_fds[1]);
                } 
                // リダイレクト処理ここに追加！
                if (handle_redirection(command_tokens) == -1)
                // エラーメッセージは handle_redirection 内で出力済み
                    exit(EXIT_FAILURE);

                execute_command(command_tokens);
                
            } else { // 親プロセス内の処理
                if (prev_fd != -1) {
                    close(prev_fd);
                }
                if (i < num_commands - 1) {
                    close(pipe_fds[1]);
                    prev_fd = pipe_fds[0];
                }
            }
        }
    }

    // すべての子プロセスの終了を待つ
    while (wait(&status) > 0)
        ;

    return 0;
}





