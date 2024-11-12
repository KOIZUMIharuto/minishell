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

// 内部コマンドかををチェックし、内部コマンドの場合はインデックスを返す
int is_builtin_mark_index(char *cmd) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(cmd, builtins[i].name) == 0)
            return i;
    }
    return 0;
}

int execute_builtin(char **command, int index) {
    int backup_stdout = -1;
    int redirect_fd;

    // リダイレクトの有無をチェック
    for (int i = 0; command[i] != NULL; i++) {
        if (strcmp(command[i], ">") == 0 && command[i + 1] != NULL) {
        	// 標準出力をバックアップ
            backup_stdout = dup(STDOUT_FILENO);
            if (backup_stdout == -1) {
                perror("dup");
                return -1;
            }
        }
    }

    // リダイレクトの処理
    redirect_fd = handle_redirection(command);
    if (redirect_fd == -1) {
        if (backup_stdout != -1) {
            close(backup_stdout);
        }
        return -1;
    }  

    // コマンドを実行
    builtins[index].func(command);


    // 標準出力を元に戻す
    if (backup_stdout != -1) {
        if (dup2(backup_stdout, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(backup_stdout);
            return -1;
        }
        close(backup_stdout);
    }
    
    return 0;
}

  
void interpret_line(char *line) {
    int is_builtin_index;
    char **tokens = ft_split(line, ' ');
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        return;
    }

    // 内部コマンドの実行または外部コマンドの実行
    if (tokens[0] != NULL) {
        // 内部コマンドかを判断、内部コマンドの場合はインデックスを渡す
        is_builtin_index = is_builtin_mark_index(tokens[0]);
        if (is_builtin_index) {
            execute_builtin(tokens,is_builtin_index);
        } else {
            my_execve(tokens);
        }
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