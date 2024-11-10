// pwd.c
#include "minishell.h"

void builtin_pwd() {
    char *buffer;
    size_t size = 1024; // 初期バッファサイズ（必要に応じて調整）

    // メモリを確保
    buffer = (char *)malloc(size);
    if (buffer == NULL) {
        perror("malloc");
        return;
    }

    // getcwd で現在の作業ディレクトリを取得
    if (getcwd(buffer, size) == NULL) {
        perror("getcwd");
        free(buffer); // メモリを解放
        return;
    }

    // 現在の作業ディレクトリを出力
    printf("%s\n", buffer);

    // メモリを解放
    free(buffer);
}

