#include "minishell.h"

// リダイレクト用のファイルをオープンする関数
int open_redirect_file(char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror(filename); 
        return -1;
    }
    return fd;
}

// リダイレクトを検出して処理する関数
int handle_redirection(char **tokens) {
    int redirect_fd = -1;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], ">") == 0 && tokens[i + 1] != NULL) {
            // リダイレクト先のファイルを親プロセスでオープン
            redirect_fd = open_redirect_file(tokens[i + 1]);
            if (redirect_fd == -1) {
                return -1;  // エラー発生
            }
            
            // リダイレクト記号とファイル名を削除
            free(tokens[i]);
            tokens[i] = NULL;
            free(tokens[i + 1]);
            tokens[i + 1] = NULL;
            break;
        }
    }
    
    return redirect_fd;
}