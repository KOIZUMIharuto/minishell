// unset.c
#include "minishell.h"

/**
 * remove_env - 環境変数を削除します。
 * @var: 削除する変数名
 */
void remove_env(const char *var) {

    size_t var_len = strlen(var);
    for (char **env = environ; *env != NULL; env++) {
        if (strncmp(*env, var, var_len) == 0 && (*env)[var_len] == '=') {
            free(*env); // 古いエントリを解放

            // 環境変数をシフトして配列を更新
            for (char **next = env; *next != NULL; next++) {
                *next = *(next + 1);
            }
            break;
        }
    }
}


/**
 * builtin_unset - unset コマンドを実装します。
 * @args: コマンドライン引数の配列
 */
void builtin_unset(char **args) {
    if (args[1] == NULL) {
        const char *error_msg = "unset: not enough arguments\n";
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        remove_env(args[i]);
    }
}

