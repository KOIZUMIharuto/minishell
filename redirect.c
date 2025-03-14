// redirect.c
#include <minishell.h>

// リダイレクト用のファイルをオープンする関数
int open_redirect_file(char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror(filename); 
        return -1;
    }
    return fd;
}

int handle_redirection(char **command) {
    int redirect_fd; 

    redirect_fd = -1;
    // 標準出力の上書きリダイレクト ">"
    for (int i = 0; command[i] != NULL; i++) {
        if (((ft_strcmp(command[i], ">") == 0 || ft_strcmp(command[i], ">>") == 0) || ft_strcmp(command[i], "<") == 0) && command[i + 1] != NULL) {
            if (ft_strcmp(command[i], ">") == 0) 
            {
                redirect_fd = open(command[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else if (ft_strcmp(command[i], ">>") == 0)
            {
                redirect_fd = open(command[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else if (ft_strcmp(command[i], "<") == 0)
            {
                redirect_fd = open(command[i + 1], O_RDONLY);
            }
            if (redirect_fd == -1) {
                perror(command[i + 1]); 
                return -1;
            }
            if (ft_strcmp(command[i], ">") == 0 || ft_strcmp(command[i], ">>") == 0)
            {   // 標準出力をリダイレクト
                if (dup2(redirect_fd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    close(redirect_fd);
                    return -1;
                }
                close(redirect_fd);
            }
            else if (ft_strcmp(command[i], "<") == 0)
            {   // 標準入力をリダイレクト
                if (dup2(redirect_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    close(redirect_fd);
                    return -1;
                }
                close(redirect_fd);
            }
            // リダイレクト演算子とファイル名をコマンドから削除
            free(command[i]);
            free(command[i + 1]);
            command[i] = NULL;
            command[i + 1] = NULL;
            break;
        }
    }
    return 0; // 正常終了
}