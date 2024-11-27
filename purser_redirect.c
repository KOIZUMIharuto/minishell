/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:15:15 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/26 14:26:04 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>
#include <get_next_line.h>

static t_redirection	**check_redirection(char *line, char *key, int redirect_cnt);
static int				set_redirection_type(char *key_pos, char key, t_redirection *redirection);

char	**recursive_split(char *line, char *del, int word_cnt)
{
	char	**tokens;
	char	*token;
	t_quote	quote;
	int		i;

	while (*line && is_del(*line, del, NULL))
		line++;
	if (!*line)
	{
		tokens = (char **)ft_calloc(word_cnt + 1, sizeof(char *));
	}
	else
	{
		quote = NONE_QUOTE;
		i = 0;
		while (line[i] && !is_del(line[i], del, &quote))
			i++;
		token = ft_substr(line, 0, i);
		if (!token)
			return (NULL);
		tokens = recursive_split(line + i, del, word_cnt + 1);
		if (!tokens)
			free(token);
		else
			tokens[word_cnt] = token;
	}
	return (tokens);
}

t_cmd	**recursive_purser(char **tokens, int cmd_cnt)
{
	t_cmd	**cmds;
	t_cmd	*cmd;

	if (!tokens[cmd_cnt])
	{
		cmds = (t_cmd **)ft_calloc(cmd_cnt + 1, sizeof(t_cmd *));
		if (!cmds)
			return (NULL);
	}
	else
	{
		cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!cmd)
			return (NULL);
		cmd->input_redurection = check_redirection(tokens[cmd_cnt], "<", 0);
		cmd->output_redurection = check_redirection(tokens[cmd_cnt], ">", 0);
		cmd->args = split_arg(tokens[cmd_cnt]);
		cmd->cmd = recursive_split(tokens[cmd_cnt], " \t\n", 0);
		cmd->infile_fd = -1;
		cmd->outfile_fd = -1;
		if (!cmd->input_redurection || !cmd->output_redurection || !cmd->cmd)
		{
			free_cmd(cmd);
			return (NULL);
		}
		cmds = recursive_purser(tokens, cmd_cnt + 1);
		if (!cmds)
		{
			free_cmd(cmd);
			return (NULL);
		}
		cmds[cmd_cnt] = cmd;	
	}
	return (cmds);
}

static t_redirection	**check_redirection(char *line, char *key, int redirect_cnt)
{
	t_redirection		**redirections;
	t_redirection		*redirection;
	t_quote				quote;
	char				*key_pos;
	char				*key_pos_tmp;
	int					i;

	if (!line)
		return (NULL);
	key_pos = line;
	while (*key_pos && !is_del(*key_pos, key, NULL))
			key_pos++;
	if (!*key_pos)
	{
		redirections = (t_redirection **)ft_calloc(redirect_cnt + 1, sizeof(t_redirection *));
		if (!redirections)
			return (NULL);
	}
	else
	{
		redirection = (t_redirection *)ft_calloc(1, sizeof(t_redirection));
		if (!redirection)
			return (NULL);
		key_pos_tmp = key_pos++;
		key_pos += set_redirection_type(key_pos, key[0], redirection);
		while (*key_pos && is_del(*key_pos, " \t\n", NULL))
			key_pos++;
		quote = NONE_QUOTE;
		i = 0;
		while (key_pos[i] && !is_del(key_pos[i], " \t\n", &quote))
			i++;
		if (key_pos[0])
		{
			redirection->file = ft_substr(key_pos, 0, i);
			if (!redirection->file)
			{
				free(redirection);
				return (NULL);
			}
		}
		else
			redirection->file = NULL;
		redirection->fd = -1;
		(void)ft_strlcpy(key_pos_tmp, key_pos + i, ft_strlen(key_pos_tmp) + 1);
		redirections = check_redirection(line, key, redirect_cnt + 1);
		if (!redirections)
		{
			free_redirection(redirection);
			return (NULL);
		}
		redirections[redirect_cnt] = redirection;
	}
	return (redirections);
}

static int	set_redirection_type(char *key_pos, char key, t_redirection *redirection)
{
	if (key == '<')
	{
		if (key_pos[0] == '<')
		{
			redirection->type = HEREDOCUMENT;
			return (1);
		}
		else
			redirection->type = INPUT_REDIRECTION;
	}
	else
	{
		if (key_pos[0] == '>')
		{
			redirection->type = APPEND_REDIRECTION;
			return (1);
		}
		else
			redirection->type = OVERWRITE_REDIRECTION;
	}
	return (0);
}