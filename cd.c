#include "minishell.h"

void builtin_cd(char **args) {
    // 引数が指定されていない場合のエラー処理
    if (args[1] == NULL) {
        fprintf(stderr, "cd: path required\n");
    }
    // ディレクトリ変更に失敗した場合のエラー処理
    else if (chdir(args[1]) != 0) {
        perror("cd");
    }
}
