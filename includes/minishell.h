/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:46 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 00:47:26 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // 自作ライブラリの関数群を使用するためのヘッダー
# include <limits.h> // システムの限界値（INT_MAX、PATH_MAX など）を定義するためのヘッダー
# include <stdlib.h> // 標準ライブラリ。メモリ管理関数（malloc, free）や乱数生成、環境変数操作（getenv）などを提供
# include <unistd.h> // POSIX標準の関数（write, read, fork, execve など）を使用するためのヘッダー
# include <sys/stat.h> // ファイルの情報を取得するための `stat` 構造体や関数を使用するためのヘッダー
# include <stdio.h> // 標準入出力関数（printf, perror など）を使用するためのヘッダー
# include <signal.h> // シグナル処理のためのヘッダー
# include <string.h> // 文字列操作関数（strcpy, strcat, strlen など）を使用するためのヘッダー
# include <readline/readline.h> // GNU Readline ライブラリを使用して、コマンドライン入力を扱うためのヘッダー
# include <readline/history.h> // Readline の履歴機能を使用するためのヘッダー
# include <sys/wait.h>  // waitpid を使用するために必要なヘッダー
# include <fcntl.h> // open, close, O_RDONLY, O_WRONLY, O_CREAT を使用するために必要なヘッダー
# include <errno.h>
# include <stdbool.h>

extern char **environ;
extern int g_last_exit_status;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;


t_list	*env_convert(char **env);
void	env_delete(t_list **env_list, char *key);
void	env_free(void *content);
t_env	*env_get(t_list *env_list, char *key);
bool	env_update(t_list **env_list, char *env);

// 内部コマンドのプロトタイプ宣言
int	builtin_echo(char **args);
int	builtin_cd(char **args);
int	builtin_pwd(char **args);
int	builtin_export(char **args);
int	builtin_unset(char **args);
int	builtin_env(char **args);
int	builtin_exit(char **args);

int		execute_builtin(char **command, int index);
int		is_builtin_mark_index(char *cmd);
void	execute_command(char **command);

int		handle_redirection(char **command);
int		handle_heredocument(char **command);
char	*find_command(char *command);
void	signal_handler(int signum);
int		pipe_command(char **commands);

int is_valid_identifier(char *name);
void handle_invalid_identifier(char *command,char *name);

void free_double_pointor(char **array);

void setup_signal_handlers(void);


#endif