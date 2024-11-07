#include "minishell.h"

// ファイルが実行可能かを確認する関数
int is_executable(char *path) {
    struct stat sb;
    // `stat` 関数でファイル情報を取得し、取得に成功した場合に
    // そのファイルが通常ファイルかつ実行可能かを確認
    return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR));
}

// コマンドのパスを探す関数
char *find_command(char *command) {
    // 絶対パスまたは相対パスで指定されているかを確認
    if (strchr(command, '/') != NULL) {
        // 絶対パスや相対パスで指定されている場合、そのまま実行可能か確認
        if (is_executable(command)) {
            // 実行可能な場合はそのパスを複製して返す
            char *result = (char *)malloc(strlen(command) + 1);
            if (result != NULL) {
                strcpy(result, command);
            }
            return result;
        }
        // 実行不可能な場合は NULL を返す
        return NULL;
    }

    // 環境変数 PATH を取得
    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        // PATH 環境変数が取得できない場合、NULL を返す
        return NULL;
    }

    // `:` で PATH を分割してディレクトリのリストを取得
    char **path_dirs = ft_split(path_env, ':');
    if (path_dirs == NULL) {
        // メモリ割り当てに失敗した場合、NULL を返す
        return NULL;
    }

    // 各ディレクトリを走査してコマンドのフルパスを構築し、実行可能か確認
    for (int i = 0; path_dirs[i] != NULL; i++) {
        // フルパスのバッファを確保
        char *full_path = (char *)malloc(strlen(path_dirs[i]) + strlen(command) + 2);
        if (full_path == NULL) {
            // メモリ割り当てに失敗した場合、メモリ解放して NULL を返す
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return NULL;
        }

        // ディレクトリパスとコマンド名を結合してフルパスを構築
        strcpy(full_path, path_dirs[i]);
        strcat(full_path, "/");
        strcat(full_path, command);

        // 実行可能か確認
        if (is_executable(full_path)) {
            // 実行可能なパスが見つかった場合、使用済みメモリを解放してフルパスを返す
            for (int j = 0; path_dirs[j] != NULL; j++) {
                free(path_dirs[j]);
            }
            free(path_dirs);
            return full_path;
        }

        // 実行不可能な場合、バッファを解放
        free(full_path);
    }

    // すべてのディレクトリを調べても見つからなかった場合、メモリを解放して NULL を返す
    for (int i = 0; path_dirs[i] != NULL; i++) {
        free(path_dirs[i]);
    }
    free(path_dirs);
    return NULL;
}
