#include "minishell.h"

/**
 * print_env - 環境変数をすべて出力する関数
 */
void print_env(void) {
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

/**
 * builtin_env - `env` コマンドを実装
 * @args: コマンドライン引数の配列（この実装では無視）
 */
void builtin_env() {
    print_env();
}