#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // 自作ライブラリの関数群を使用するためのヘッダー
# include <limits.h> // システムの限界値（INT_MAX、PATH_MAX など）を定義するためのヘッダー
# include <stdlib.h> // 標準ライブラリ。メモリ管理関数（malloc, free）や乱数生成、環境変数操作（getenv）などを提供
# include <unistd.h> // POSIX標準の関数（write, read, fork, execve など）を使用するためのヘッダー
# include <sys/stat.h> // ファイルの情報を取得するための `stat` 構造体や関数を使用するためのヘッダー
# include <stdio.h> // 標準入出力関数（printf, perror など）を使用するためのヘッダー
# include <string.h> // 文字列操作関数（strcpy, strcat, strlen など）を使用するためのヘッダー
# include <readline/readline.h> // GNU Readline ライブラリを使用して、コマンドライン入力を扱うためのヘッダー
# include <readline/history.h> // Readline の履歴機能を使用するためのヘッダー


char *find_command(char *command);

// 環境変数の定義
extern char **environ;

#endif