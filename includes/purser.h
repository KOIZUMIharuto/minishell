/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:15:37 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/26 14:24:35 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PURSER_H
# define PURSER_H

# include "minishell.h"
# include <stdbool.h>

typedef enum s_quote
{
    NONE_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} 	t_quote;

typedef enum s_redirection_type
{
	NONE_REDIRECTION,
	OVERWRITE_REDIRECTION,
	APPEND_REDIRECTION,
	INPUT_REDIRECTION,
	HEREDOCUMENT
}	t_redirection_type;

typedef struct s_redirection
{
	t_redirection_type	type;
	char				*file;
	int					fd;
}	t_redirection;

typedef struct s_cmd
{
	t_list			*args;
	char			**cmd;
	t_redirection	**input_redurection;
	t_redirection	**output_redurection;
	int				infile_fd;
	int				outfile_fd;
}	t_cmd;

t_cmd	**purser(char *line);
bool	is_del(char c, char *del, t_quote *quote);

char	**recursive_split(char *line, char *del, int word_cnt);
t_cmd	**recursive_purser(char **tokens, int cmd_cnt);

t_list	*split_arg(char *line);

void	free_redirections(t_redirection **redirections);
void	free_redirection(t_redirection *redirection);
void	free_cmds(t_cmd **cmds);
void	free_cmd(t_cmd *cmd);
void	free_tokens(char **tokens);

#endif