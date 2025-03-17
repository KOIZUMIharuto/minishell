/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:46 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 14:32:54 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # define _GNU_SOURCE

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

# define BUILTIN_NUM 7
# define PROMPT "minishell$ "

char **environ;
extern int g_last_exit_status;

typedef struct s_env
{
	char	*key;
	char	*value;
	bool	is_shell_var;
}	t_env;

typedef enum e_valid
{
	VALID,
	INVALID,
	ERROR
}	t_valid;

typedef enum e_quote
{
	NONE_Q,
	SINGLE_Q,
	DOUBLE_Q
}	t_quote;

typedef enum s_rdrct_type
{
	NONE_RDRCT,
	OVERWRITE_RDRCT,
	APPEND_RDRCT,
	INPUT_RDRCT,
	HEREDOCUMENT
}	t_rdrct_type;

typedef struct s_rdrct
{
	t_rdrct_type	type;
	char			**file;
	int				fd;
}	t_rdrct;

typedef struct s_cmd
{
	char	**cmd;
	t_rdrct	**input_rdrct;
	t_rdrct	**output_rdrct;
	int		infile_fd;
	int		outfile_fd;
    int     backup_stdin;   // 標準入力のバックアップfd用
    int     backup_stdout;  // 標準出力のバックアップfd用
}	t_cmd;

typedef struct s_purser
{
	char	exit_status[4];
	t_list	*env;
	char	*tmp;
}	t_purser;

typedef struct s_builtin
{
	char	*name;
	int		(*func)(char **, t_list *);
}	t_builtin;

// env
t_list	*env_init(char **env);
t_valid	env_split(char *env, char **key, char **value);
bool	env_delete(t_list **env_list, char *key);
void	env_free(void *content);
t_env	*env_get(t_list *env_list, char *key, bool even_if_shell_var);
bool	env_update(t_list **env_list, char *key, char *value);
bool	is_valid_key(char *key);
bool	print_invalid_key(char *cmd, char *key);


// puerser
t_cmd	**purser(char *line, int exit_status, t_list *env);
bool	is_del(char c, char *del, t_quote *quote);
t_rdrct	**check_rdrct(char *line, char *key, int rdrct_cnt, t_purser *data);
char	**split_arg(char *line, t_purser *data);
char	*recursive_expand(char **arg, t_purser *data, t_quote quote, int len);

void	free_rdrcts(t_rdrct **rdrcts, int i);
void	free_rdrct(t_rdrct *rdrct);
void	free_cmds(t_cmd **cmds, int i);
void	free_cmd(t_cmd *cmd);
void	free_tokens(char **tokens);

// builtin
void	init_builtins(t_builtin *builtins);
int		get_builtin_index(t_builtin *builtins, char *cmd);
int		builtin_echo(char **cmd, t_list *env);
int		builtin_cd(char **cmd, t_list *env);
int		builtin_pwd(char **cmd, t_list *env);
char	*get_pwd(void);
int		builtin_export(char **cmd, t_list *env);
int		builtin_unset(char **cmd, t_list *env);
int		builtin_env(char **cmd, t_list *env);
int		builtin_exit(char **cmd, t_list *env);

int execute_builtin(t_cmd *cmd, int (*func)(char **, t_list *), t_list *env);
char **convert_env_list_to_array(t_list *env);

int handle_redirection(t_cmd *cmd);
int handle_heredocument(char *delimiter, t_cmd *cmd);
int restore_redirection(t_cmd *cmd);

int execute_pipeline(t_cmd **cmds,t_builtin *builtins,t_list *env);
void execute_cmd(char **cmd, t_list *env);

int		perror_int(char *msg, int errnum);
bool	perror_bool(char *msg, int errnum);
void	*perror_ptr(char *msg, int errnum);
void	my_perror(char *msg, int errnum);

void free_double_pointor(char **array);

void setup_interactive_signals(void);
void setup_builtin_signals(void);
void setup_exec_signals(void);
void setup_child_signals(void);


#endif