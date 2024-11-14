// pipe.c
#include "minishell.h"

// パイプ処理を行う関数
// ls | grep .c | wc -l の様なパイプ処理を行う
int pipe_command(char **commands) {
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
        
        char **command_tokens = ft_split(commands[i], ' ');

        // 組み込みコマンドの判定
        int is_builtin_index = is_builtin_mark_index(command_tokens[0]);
        if (is_builtin_index) {;
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
            execute_builtin(command_tokens, is_builtin_index);

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





