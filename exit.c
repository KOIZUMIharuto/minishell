// exit.c
#include "minishell.h"

/**
 * builtin_exit - exit コマンドを実装します（引数なしを想定）。
 */
void builtin_exit() {
    // 終了メッセージを出力し、シェルを終了
    printf("exit\n");
    exit(0); // 常にステータス 0 で終了
}

