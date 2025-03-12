// main.c
#include "includes/minishell.h"
  
int interpret_line(char *line) {
    char **tokens = ft_split(line, '|'); // パイプで分割
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        return (0);
    }
    // 内部コマンドの実行または外部コマンドの実行
    int status;
    if (tokens[0] != NULL) {
        // 内部コマンドかを判断、内部コマンドの場合はインデックスを渡す
        status=pipe_command(tokens);
    }
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
    return (status);
}

int g_last_exit_status = 0;

// メイン関数
int main() {
    char *line;

    struct sigaction sa;
    sa.sa_handler = signal_handler; // ハンドラ関数を指定
    sigemptyset(&sa.sa_mask);       //  ブロックするシグナルを空に設定
    sa.sa_flags = SA_RESTART;       // 中断されたシステムコールを再開

    // SIGINT (Ctrl+C) の設定
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up sigaction for SIGINT");
        return 1;
    }

    // SIGQUIT (Ctrl+\) の動作を無視するように設定
    sa.sa_handler = SIG_IGN; // 無視する設定
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("Error setting up sigaction for SIGQUIT");
        return 1;
    }

    while (1) {
        line = readline("$ ");
        if (line == NULL) {  // EOF (Ctrl+D)
            printf("\n");
            break;
        }
        if (strlen(line) > 0) {
            add_history(line);
            int status=interpret_line(line);
            // exit コマンドが実行された場合
            if (status == -42) {
                free(line);
                exit(g_last_exit_status);
            }
        }
        free(line);
    }

    return 0;
}

