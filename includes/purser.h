/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:15:37 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/02/07 13:58:34 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PURSER_H
# define PURSER_H

# include "minishell.h"
# include <stdbool.h>

typedef enum e_quote
{
	NONE_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum s_rdrct_type
{
	NONE_rdrct,
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
	t_rdrct	**INPUT_RDRCT;
	t_rdrct	**output_rdrct;
	int		infile_fd;
	int		outfile_fd;
}	t_cmd;

t_cmd	**purser(char *line);
bool	is_del(char c, char *del, t_quote *quote);

t_rdrct	**check_rdrct(char *line, char *key, int rdrct_cnt);

char	**split_arg(char *line);
char	*recursive_expand(char **arg, char **env, t_quote quote, int len);


void	free_rdrcts(t_rdrct **rdrcts, int i);
void	free_rdrct(t_rdrct *rdrct);
void	free_cmds(t_cmd **cmds, int i);
void	free_cmd(t_cmd *cmd);
void	free_tokens(char **tokens);

#endif