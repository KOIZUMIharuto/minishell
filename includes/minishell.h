/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:46 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/04/01 18:53:36 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define BUILTIN_NUM 7
# define PROMPT "minishell$ "

extern int			g_last_exit_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			is_hidden;
}					t_env;

typedef enum e_valid
{
	EXIT_MAINT_LOOP = -42,
	SIGINT_EXIT = -24,
	CMD_EXTERNAL = -2,
	CRITICAL_ERROR = -1,
	VALID = 0,
	INVALID = 1
}					t_valid;

typedef enum e_quote
{
	NONE_Q,
	SINGLE_Q,
	DOUBLE_Q
}					t_quote;

typedef enum s_redirect_type
{
	NONE_RDRCT,
	OVERWRITE_RDRCT,
	APPEND_RDRCT,
	INPUT_RDRCT,
	HEREDOCUMENT
}					t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type	type;
	char			*token;
	char			**file;
	bool			is_quoted;
	int				fd;
}					t_redirect;

typedef struct s_cmd
{
	char			**cmd;
	t_redirect		**redirects;
	int				infile_fd;
	int				outfile_fd;
	int				backup_stdin;
	int				backup_stdout;
	int				original_stdin;
}					t_cmd;

typedef struct s_parser
{
	char			exit_status[4];
	t_list			*env;
	char			*del;
	bool			is_env_empty;
	bool			is_failed;
	char			*tmp;
}					t_parser;

typedef struct s_builtin
{
	char			*name;
	t_valid((*func)(char **, t_list *));
}					t_builtin;

typedef struct s_pipe_info
{
	int				prev_fd;
	int				pipe_fds[2];
	bool			has_next;
}					t_pipe_info;

typedef struct s_data
{
	t_list			*env;
	t_cmd			**cmds;
	pid_t			*pids;
}					t_data;

void				setup_interactive_signals(void);
void				setup_builtin_signals(void);
void				setup_exec_signals(void);
void				setup_child_signals(void);
void				setup_heredoc_signals(void);
void				setup_after_rl_signals(void);

t_valid				setup_pipe(t_pipe_info *pipe_info, bool has_next);
t_valid				execute_pipeline(t_cmd **cmds, t_builtin *builtins,
						t_list *env);
t_valid				exec_single_builtin(t_cmd *cmd,
						t_valid (*builtin_func)(char **, t_list *),
						t_data data);
t_valid				exec_cmds(t_builtin *builtins, t_data *data,
						t_pipe_info *pipe_info);
void				exec_cmd_redirect(t_cmd *cmd, t_pipe_info *pipe_info,
						t_data data);
void				execute_cmd(char **cmd, t_data data);
void				manage_pipes(t_pipe_info *pipe_info);
t_valid				handle_pipe_input(t_pipe_info *pipe_info);
t_valid				handle_pipe_output(t_pipe_info *pipe_info);

t_valid				process_heredocs(t_cmd *cmd, t_list *env);
int					write_expand_env(int pipe_fd, char *line, t_list *env_list);
t_valid				wait_heredoc(int pipe_fds[2], t_cmd *cmd, pid_t pid);
t_valid				handle_redirection(t_cmd *cmd, t_list *env);
t_valid				restore_redirection(t_cmd *cmd);

void				init_builtins(t_builtin *builtins);
int					get_builtin_index(t_builtin *builtins, char *cmd);
t_valid				builtin_echo(char **cmd, t_list *env);
t_valid				builtin_cd(char **cmd, t_list *env);
t_valid				builtin_pwd(char **cmd, t_list *env);
char				*get_pwd(void);
t_valid				builtin_export(char **cmd, t_list *env);
t_valid				builtin_unset(char **cmd, t_list *env);
t_valid				builtin_env(char **cmd, t_list *env);
t_valid				builtin_exit(char **cmd, t_list *env);

t_list				*env_init(char **env);
t_valid				env_split(char *env, char **key, char **value,
						t_list *env_list);
void				env_delete(t_list **env_list, char *key);
void				env_free(void *content);
t_env				*env_get(t_list *env_list, char *key,
						bool even_if_shell_var);
bool				env_update(t_list **env_list, char *key, char *value);
t_valid				is_valid_key(char *key);
char				**convert_env_list_to_array(t_list *env);

t_valid				parser(t_cmd ***cmds, char *line, int exit_status,
						t_list *env);
bool				tokenize(t_list **tokens, char *line);
t_valid				check_syntax(t_list *tokens);
bool				split_tokens(t_list ***splited_tokens, t_list *tokens);
t_list				*to_cmd_list(t_list **splited_tokens, t_parser data);
bool				get_redirects(t_redirect ***redirects, t_list **tokens,
						t_parser data);
bool				get_redirect_list(t_list **redirect_list, t_list **tokens);
bool				expand_env_quote(t_list **expanded, char *token,
						t_parser *data);
char				*recursive_expand(char **token, t_parser *data,
						t_quote quote, int len);
bool				is_in_quote(char c, t_quote *quote, bool is_include_quote);
bool				is_effective_quote(char c, t_quote *quote);

void				free_cmds(t_cmd **cmds);
void				free_cmd(void *content);
void				free_redirects(void *content);
void				free_redirect(void *content);

t_valid				error_msg(char *cmd, char *msg, t_valid is_valid);
int					perror_int(char *cmd);
bool				perror_bool(char *cmd);
void				*perror_ptr(char *cmd);

void				close_wrapper(int *fd);
bool				print_msg(char *msg, int fd);

void				free_double_pointor(char **array);
void				free_data(t_data data);

#endif
