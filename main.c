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
int execute_builtin(char **args, int redirect_fd) {
    int backup_stdout = -1;
    int builtin_executed = 0;
    
    // リダイレクトが指定されている場合
    if (redirect_fd != -1) {
        // 現在の標準出力をバックアップ
        backup_stdout = dup(STDOUT_FILENO);
        if (backup_stdout == -1) {
            perror("dup");
            return 0;  // 内部コマンドではないとして扱う
        }
        
        // 標準出力をリダイレクト先に変更
        if (dup2(redirect_fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(backup_stdout);
            return 0;  // 内部コマンドではないとして扱う
        }
    }

    // コマンドを実行
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            builtin_executed = 1;
            break;
        }
    }

    // 標準出力を元に戻す
    if (redirect_fd != -1) {
        if (dup2(backup_stdout, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(backup_stdout);
            return 0;  // 内部コマンドではないとして扱う
        }
        if (close(backup_stdout) == -1) {
            perror("close");
            return 0;  // 内部コマンドではないとして扱う
        }
    }

    return builtin_executed;  // 内部コマンドが見つかった場合は1、見つからなかった場合は0を返す
}

  
void interpret_line(char *line) {
    char **tokens = ft_split(line, ' ');
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        return;
    }

    // リダイレクトの存在確認
    int redirect_fd = -1;
    int has_redirect = 0;
    
    // トークンを走査してリダイレクトを探す
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], ">") == 0 && tokens[i + 1] != NULL) {
            has_redirect = 1;
            break;
        }
    }

    // リダイレクトがある場合のみ処理
    if (has_redirect) {
        redirect_fd = handle_redirection(tokens);
        if (redirect_fd == -1) {
            // エラーメッセージは handle_redirection で出力済み
            for (int i = 0; tokens[i] != NULL; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return;
        }
    }

    // 内部コマンドの実行または外部コマンドの実行
    if (tokens[0] != NULL) {
        int is_builtin = execute_builtin(tokens, redirect_fd);
        if (is_builtin == 0) {  // 内部コマンドが見つからなかった場合は外部コマンドを実行
            my_execve(tokens, redirect_fd);
        }
    }

    // リソースのクリーンアップ
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