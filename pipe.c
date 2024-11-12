// pipe.c
#include "minishell.h"

// リダイレクト処理を行う関数（一旦除外）
        // // リダイレクト処理
        // if (handle_redirection(commands) == -1)
        //     // エラーメッセージは handle_redirection 内で出力済み
        //     exit(EXIT_FAILURE);



// パイプ処理を行う関数
// ls | grep .c | wc -l の様なパイプ処理を行う
int pipe_command(char **commands) {
    int pipe_fds[2];  // 1つのパイプ用の配列
    int prev_fd = -1; // 前のプロセスの出力ファイルディスクリプタ
    int pid;
    int status;

    // 最初のコマンド (ls)
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 標準出力をパイプの書き込み側に接続
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        char **command1 = ft_split(commands[0], ' ');
        execute_command(command1);
        exit(EXIT_SUCCESS);
    }

    close(pipe_fds[1]);
    prev_fd = pipe_fds[0];

    // 2番目のコマンド (grep)
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 標準入力を前のパイプの読み込み側に接続
        dup2(prev_fd, STDIN_FILENO);
        // 標準出力を現在のパイプの書き込み側に接続
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        char **command2 = ft_split(commands[1], ' ');
        execute_command(command2);
        exit(EXIT_SUCCESS);
    }

    close(prev_fd);
    close(pipe_fds[1]);
    prev_fd = pipe_fds[0];

    // 3番目のコマンド (wc)
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 標準入力を前のパイプの読み込み側に接続
        dup2(prev_fd, STDIN_FILENO);
        close(pipe_fds[0]);
        char **command3 = ft_split(commands[2], ' ');
        execute_command(command3);
        exit(EXIT_SUCCESS);
    }

    close(prev_fd);

    // すべての子プロセスの終了を待つ
    while (wait(&status) > 0)
        ;

    return 0;
}
