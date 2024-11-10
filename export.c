// export.c
#include "minishell.h"

/**
 * is_valid_identifier - 変数名が有効な識別子かどうかをチェックします。
 * @name: チェックする変数名
 * 
 * return: 有効な場合は1を、無効な場合は0を返します。
 */
int is_valid_identifier(const char *name) {
    // NULL または空文字列は無効
    if (name == NULL || *name == '\0') {
        return 0;
    }
    // 最初の文字はアルファベットまたはアンダースコアである必要があります
    if (!isalpha(name[0]) && name[0] != '_') {
        return 0;
    }
    // 残りの文字はアルファベット、数字、アンダースコアのみ許可
    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_') {
            return 0;
        }
    }
    return 1;
}

/**
 * handle_invalid_identifier - 無効な変数名に対するエラーメッセージを出力します。
 * @name: 無効な変数名
 */
void handle_invalid_identifier(const char *name) {
    const char *error_msg = "export: `";
    // 標準エラー出力にエラーメッセージを出力
    write(STDERR_FILENO, error_msg, strlen(error_msg));
    write(STDERR_FILENO, name, strlen(name));
    write(STDERR_FILENO, "': not a valid identifier\n", strlen("': not a valid identifier\n"));
}

/**
 * add_or_update_env - 環境変数を追加または更新します。
 * @var: 環境変数の名前
 * @value: 環境変数の値
 * 
 * 変数名が無効な場合はエラーメッセージを出力し、処理を中断します。
 */
void add_or_update_env(const char *var, const char *value) {
    // 変数名のバリデーションチェック
    if (!is_valid_identifier(var)) {
        handle_invalid_identifier(var);
        return;
    }

    // 変数名と値の長さを取得
    size_t var_len = strlen(var);
    size_t value_len = value ? strlen(value) : 0;

    // "VAR=VALUE" または "VAR" の形式の新しいエントリを作成
    char *new_entry;
    if (value) {
        // メモリを割り当てて文字列を作成 ("VAR=VALUE" の形式)
        new_entry = malloc(var_len + value_len + 2); // '=' と NULL のために +2
        if (new_entry == NULL) {
            perror("malloc");
            return;
        }
        sprintf(new_entry, "%s=%s", var, value);
    } else {
        // 値がない場合は "VAR" のみのエントリを作成
        new_entry = malloc(var_len + 1); // NULL のために +1
        if (new_entry == NULL) {
            perror("malloc");
            return;
        }
        strcpy(new_entry, var);
    }

    // 既存の環境変数を検索して更新する
    for (char **env = environ; *env != NULL; env++) {
        if (strncmp(*env, var, var_len) == 0 && ((*env)[var_len] == '=' || (*env)[var_len] == '\0')) {
            free(*env); // 古いエントリを解放
            *env = new_entry; // 新しいエントリを設定
            return;
        }
    }

    // 環境変数の数を数える
    size_t env_count = 0;
    while (environ[env_count] != NULL) {
        env_count++;
    }

    // 環境変数配列を拡張するためにメモリを再割り当て
    char **new_environ = malloc((env_count + 2) * sizeof(char *));
    if (new_environ == NULL) {
        perror("malloc");
        free(new_entry);
        return;
    }

    // 既存の環境変数をコピー
    for (size_t i = 0; i < env_count; i++) {
        new_environ[i] = environ[i];
    }
    new_environ[env_count] = new_entry; // 新しいエントリを追加
    new_environ[env_count + 1] = NULL;  // NULL で終了
    environ = new_environ; // 環境変数を新しい配列に設定
}

/**
 * print_env_quoted - 環境変数を "declare -x" 形式で出力します。
 * @env: 出力する環境変数
 */
void print_env_quoted(const char *env) {
    // '=' の位置を検索
    char *equal_pos = strchr(env, '=');
    if (equal_pos) {
        // 値がある場合はダブルクオートで囲んで出力
        printf("declare -x %.*s=\"%s\"\n", (int)(equal_pos - env), env, equal_pos + 1);
    } else {
        // 値がない場合はそのまま出力
        printf("declare -x %s\n", env);
    }
}

/**
 * builtin_export - export コマンドを実装します。
 * @args: コマンドライン引数の配列
 * 
 * 引数がなければ環境変数を出力し、引数がある場合は変数を追加または更新します。
 */
void builtin_export(char **args) {
    // 引数がない場合は環境変数を出力
    if (args[1] == NULL) {
        for (char **env = environ; *env != NULL; env++) {
            print_env_quoted(*env);
        }
        return;
    }

    // 引数がある場合は変数を追加または更新
    for (int i = 1; args[i] != NULL; i++) {
        // '=' の位置を検索
        char *equal_sign = strchr(args[i], '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0'; // 変数名と値を分割
            char *var_name = args[i];
            char *var_value = equal_sign + 1;
            add_or_update_env(var_name, var_value); // 変数を追加または更新
        } else {
            add_or_update_env(args[i], NULL); // 値なしで変数を追加
        }
    }
}
