// main.c
#include "includes/minishell.h"
#include <signal.h>

// 内部コマンドの一覧
typedef struct {
    char *name;
    void (*func)(char **);
} BuiltinCommand;

// 内部コマンドのテーブル
BuiltinCommand builtins[] = {
    {"echo", builtin_echo},
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit", builtin_exit},
    {"export", builtin_export},
    {"unset", builtin_unset},
    {"env", builtin_env},
};

// 内部コマンドの数を返す関数
int num_builtins() {
    return sizeof(builtins) / sizeof(BuiltinCommand);
}

// 内部コマンドを実行する関数
int execute_builtin(char **args) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return 1;  // 実行成功
        }
    }
    return 0;  // 未定義コマンド 
}

// コマンドラインを解析し、適切な処理を行う関数
void interpret_line(char *line) {
    char **tokens = ft_split(line, ' ');
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        return;
    }

    // リダイレクトの処理
    int redirect_fd = handle_redirection(tokens);

    // 内部コマンドの実行または外部コマンドの実行
    if (tokens[0] != NULL) {
        if (!execute_builtin(tokens)) {
            my_execve(tokens, redirect_fd);  // redirect_fdを渡す
            return;  // my_execve内でリソースをクリーンアップするため、ここでreturn
        }
    }

    // builtin commandの場合のリソースのクリーンアップ
    if (redirect_fd != -1) {
        close(redirect_fd);
    }
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}


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
            interpret_line(line);
        }
        free(line);
    }

    return 0;
}