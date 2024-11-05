#include "minishell.h"

// void builtin_echo(char **args) {
//     int i = 1;
//     int newline = 1;  // デフォルトでは改行あり

//     // `-n` オプションが最初に指定されている場合の処理
//     if (args[1] && strcmp(args[1], "-n") == 0) {
//         newline = 0;
//         i = 2;  // 次の引数から出力開始
//     }

//     // 引数を順に出力
//     for (; args[i]; i++) {
//         printf("%s", args[i]);
//         if (args[i + 1]) {
//             printf(" ");
//         }
//     }

//     // 改行の有無
//     if (newline) {
//         printf("\n");
//     }
// }

#include <stdio.h>
#include <string.h>

// echo コマンドの実装
void builtin_echo(char **args) {
    int i = 1;
    int newline = 1; // 改行を出力するかどうか（1 = 出力する, 0 = 出力しない）

    // `-n` オプションが最初に指定されている場合の処理
    if (args[1] && strcmp(args[1], "-n") == 0) {
        newline = 0; // 改行を出力しない
        i = 2;       // 次の引数から出力開始
    }

    // 引数を順に出力
    for (; args[i]; i++) {
        printf("%s", args[i]);
        if (args[i + 1]) {
            printf(" "); // 引数の間にスペースを入れる
        }
    }

    // 改行の有無
    if (newline) {
        printf("\n");
    }
}



