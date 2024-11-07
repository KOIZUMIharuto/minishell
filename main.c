#include "includes/minishell.h"

// 内部コマンドのプロトタイプ宣言
void builtin_echo(char **args);
void builtin_cd(char **args);
void builtin_pwd();
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env();
void builtin_exit();

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

// 内部コマンドの数
int num_builtins() {
    return sizeof(builtins) / sizeof(BuiltinCommand);
}

// 内部コマンドを実行
int execute_builtin(char **args) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return 1;  // 実行成功
        }
    }
    return 0;  // 未定義コマンド 
}


// メイン関数
int main() {
    char *line;
    char **tokens;

    while (1) {
        line = readline("> ");
        if (line == NULL) {  // EOF (Ctrl+D)
            printf("\n");
            break;
        }
        if (strlen(line) > 0) {
            add_history(line);
        }

        // 入力の分割
        tokens = ft_split(line, ' ');

     if (tokens[0] != NULL) {
            // 内部コマンドの確認と実行
            if (!execute_builtin(tokens)) {
                // 外部コマンドのパスを検索
                char *full_path = find_command(tokens[0]);
                if (full_path != NULL) {
                    printf("Found command path: %s\n", full_path);
                    free(full_path);
                } else {
                    printf("%s: command not found\n", tokens[0]);
                }
            }
        }
        // メモリ解放
        free(line);
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}