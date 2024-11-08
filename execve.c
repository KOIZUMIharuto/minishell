#include "minishell.h"

// コマンドが実行可能かを確認する関数
int is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR));
}

// コマンドのパスを探す関数
char *find_cmd_path(char *cmd) {
    char *path_env = getenv("PATH");
    char **path_dirs;
    char *cmd_path;

    if (path_env == NULL) {
        return NULL;
    }

    path_dirs = ft_split(path_env, ':');
    if (path_dirs == NULL) {
        return NULL;
    }

    for (int i = 0; path_dirs[i] != NULL; i++) {
        cmd_path = malloc(strlen(path_dirs[i]) + strlen(cmd) + 2);
        if (cmd_path == NULL) {
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return NULL;
        }

        strcpy(cmd_path, path_dirs[i]);
        strcat(cmd_path, "/");
        strcat(cmd_path, cmd);

        if (is_executable(cmd_path)) {
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return cmd_path;
        }
        free(cmd_path);
    }

    for (int i = 0; path_dirs[i] != NULL; i++) {
        free(path_dirs[i]);
    }
    free(path_dirs);
    return NULL;
}

// 外部コマンドを実行する関数
void my_execve(char *argv_cmd) {
    char **cmd;
    char *cmd_path;
    int pid;

    cmd = ft_split(argv_cmd, ' ');
    if (cmd == NULL || cmd[0] == NULL) {
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子プロセス
        if (ft_strchr(cmd[0], '/') != NULL) {
            if (execve(cmd[0], cmd, environ) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            cmd_path = find_cmd_path(cmd[0]);
            if (cmd_path == NULL) {
                fprintf(stderr, "%s: command not found\n", cmd[0]);
                exit(EXIT_FAILURE);
            }
            if (execve(cmd_path, cmd, environ) == -1) {
                perror("execve");
                free(cmd_path);
                exit(EXIT_FAILURE);
            }
            free(cmd_path);
        }
    } else { // 親プロセス
        waitpid(pid, NULL, 0);
    }

    for (int i = 0; cmd[i] != NULL; i++) {
        free(cmd[i]);
    }
    free(cmd);
}
