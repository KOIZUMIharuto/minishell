/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:13:46 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 21:52:32 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
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
    int     backup_stdin;
    int     backup_stdout;
}	t_cmd;

typedef struct s_parser
{
	char	exit_status[4];
	t_list	*env;
	char	*tmp;
}	t_parser;

typedef struct s_builtin
{
	char	*name;
	int		(*func)(char **, t_list *);
}	t_builtin;

typedef struct s_pipe_info
{
    int prev_fd;
    int pipe_fds[2];
    bool has_next;
} t_pipe_info;

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
t_cmd	**parser(char *line, int exit_status, t_list *env);
bool	is_del(char c, char *del, t_quote *quote);
t_rdrct	**check_rdrct(char *line, char *key, int rdrct_cnt, t_parser *data);
char	**split_arg(char *line, t_parser *data);
char	*recursive_expand(char **arg, t_parser *data, t_quote quote, int len);

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

void execute_cmd(char **cmd, t_list *env);

int		error_msg(char *cmd, char *msg);
int		perror_int(char *msg, int errnum);
bool	perror_bool(char *msg, int errnum);
void	*perror_ptr(char *msg, int errnum);
void	my_perror(char *msg, int errnum);

void free_double_pointor(char **array);

void setup_interactive_signals(void);
void setup_builtin_signals(void);
void setup_exec_signals(void);
void setup_child_signals(void);

int execute_pipeline(t_cmd **cmds, t_builtin *builtins, t_list *env);
int execute_single_builtin(t_cmd *cmd, t_builtin *builtins, int builtin_index, t_list *env);
int execute_commands(t_cmd **cmds, t_builtin *builtins, t_list *env, pid_t *pids, t_pipe_info *pipe_info);
int setup_pipe(t_pipe_info *pipe_info, bool has_next);
void manage_pipes(t_pipe_info *pipe_info);
void execute_command_in_child(t_cmd *cmd, int (*builtin_func)(char **, t_list *), t_list *env, t_pipe_info *pipe_info);
int wait_for_children(pid_t *pids, int cmd_count);
int	handle_ambiguous_redirect(t_rdrct *rdrct);
void	handle_pipe_io(t_pipe_info *pipe_info);
int	process_heredocs(t_cmd *cmd);
    



#endif