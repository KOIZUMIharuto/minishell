// builin.c
# include "minishell.h"

// 内部コマンドの一覧
typedef struct {
    char *name;
    int (*func)(char **);
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

int is_builtin_mark_index(char *cmd) {
    for (int i = 0; i < num_builtins(); i++) {
        if (ft_strcmp(cmd, builtins[i].name) == 0)
            return i;
    }
    return -1;
}

int execute_builtin(char **command, int index) {
    int backup_stdout = -1;
    int redirect_fd;
    int result;

    handle_heredocument(command);

    // リダイレクトの有無をチェック
    for (int i = 0; command[i] != NULL; i++) {
        if (ft_strcmp(command[i], ">") == 0 && command[i + 1] != NULL) {
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
    result = builtins[index].func(command);


    // 標準出力を元に戻す
    if (backup_stdout != -1) {
        if (dup2(backup_stdout, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(backup_stdout);
            return -1;
        }
        close(backup_stdout);
    }
    g_last_exit_status = result;
    return result;
}

