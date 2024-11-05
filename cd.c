#include "minishell.h"

// void builtin_cd(char **args) {
//     // 引数が指定されていない場合のエラー処理
//     if (args[1] == NULL) {
//         fprintf(stderr, "cd: path required\n");
//     }
//     // ディレクトリ変更に失敗した場合のエラー処理
//     else if (chdir(args[1]) != 0) {
//         perror("cd");
//     }
// }

// cd コマンドの実装
void builtin_cd(char **args) {
    struct stat path_stat;

    // パスが指定されていない場合の処理
    if (args[1] == NULL) {
        fprintf(stderr, "cd: パスが指定されていません\n");
        return;
    }

    // パスが存在するかを確認
    if (access(args[1], F_OK) != 0) {
        perror("cd"); // パスが存在しない場合のエラー
        return;
    }

    // パスがディレクトリであるかを確認
    if (stat(args[1], &path_stat) == 0 && !S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "cd: Not a directory: %s\n", args[1]);
        return;
    }

    // chdir でディレクトリを変更
    if (chdir(args[1]) != 0) {
        perror("cd"); // アクセス権がない場合や他のエラー
    }
}
