// execve.c
#include "minishell.h"

// コマンドが実行可能かを確認する関数
int is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR));
}

// コマンドのパスを探す関数
char *find_command_path(char *command) {
    char *path_env = getenv("PATH");
    char **path_dirs;
    char *command_path;

    if (path_env == NULL) {
        return NULL;
    }

    path_dirs = ft_split(path_env, ':');
    if (path_dirs == NULL) {
        return NULL;
    }
    for (int i = 0; path_dirs[i] != NULL; i++) {
        command_path = malloc(strlen(path_dirs[i]) + strlen(command) + 2);
        if (command_path == NULL) {
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return NULL;
        }

        strcpy(command_path, path_dirs[i]);
        strcat(command_path, "/");
        strcat(command_path, command);

        if (is_executable(command_path)) {
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return command_path;
        }
        free(command_path);
    }

    for (int i = 0; path_dirs[i] != NULL; i++) {
        free(path_dirs[i]);
    }
    free(path_dirs);
    return NULL;
}

// 外部コマンドを実行する関数
void my_execve(char **command, int redirect_fd) {
    char *command_path;
    int pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        if (redirect_fd != -1) {
            close(redirect_fd);
        }
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子プロセス
        // リダイレクトが指定されている場合のみ標準出力を変更
        if (redirect_fd != -1) {
            if (dup2(redirect_fd, STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(redirect_fd);  // 不要なfdはクローズ
        }

        if (ft_strchr(command[0], '/') != NULL) {
            if (execve(command[0], command, environ) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            command_path = find_command_path(command[0]);
            if (command_path == NULL) {
                fprintf(stderr, "%s: command not found\n", command[0]);
                exit(EXIT_FAILURE);
            }
            if (execve(command_path, command, environ) == -1) {
                perror("execve");
                free(command_path);
                exit(EXIT_FAILURE);
            }
            free(command_path);
        }
    } else { // 親プロセス
        if (redirect_fd != -1) {
            close(redirect_fd);  // 親プロセスでもfdをクローズ
        }
        waitpid(pid, NULL, 0);
    }
}


