/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/14 13:59:35 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static void	init_data(t_data *data, int exit_status, char **env);
static char	**recursive_split(char *line, char *del, int word_cnt);
static bool	purse_tokens(t_cmd **cmds, char **tokens, t_data *data);

t_cmd	**purser(char *line, int exit_status, char **env)
{
	t_cmd	**cmds;
	char	**tokens;
	int		token_cnt;
	t_data	data;

	if (!line)
		return (NULL);
	tokens = recursive_split(line, "|", 0);
	if (!tokens)
		return (NULL);
	token_cnt = 0;
	while (tokens[token_cnt])
		token_cnt++;
	cmds = (t_cmd **)ft_calloc(token_cnt + 1, sizeof(t_cmd *));
	if (!cmds)
		return (NULL);
	init_data(&data, exit_status, env);
	if (!purse_tokens(cmds, tokens, &data))
	{
		free_tokens(tokens);
		free_cmds(cmds, 0);
		return (NULL);
	}
	free_tokens(tokens);
	return (cmds);
}

static void	init_data(t_data *data, int exit_status, char **env)
{
	data->exit_status = exit_status;
	data->env = env;
	data->tmp = NULL;
}

static char	**recursive_split(char *line, char *del, int word_cnt)
{
	char	**tokens;
	char	*token;
	t_quote	quote;
	int		i;

	while (*line && is_del(*line, del, NULL))
		line++;
	if (!*line)
		tokens = (char **)ft_calloc(word_cnt + 1, sizeof(char *));
	else
	{
		quote = NONE_Q;
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

static bool	purse_tokens(t_cmd **cmds, char **tokens, t_data *data)
{
	int		i;

	i = -1;
	while (tokens[++i])
	{
		cmds[i] = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!cmds[i])
			return (false);
		cmds[i]->input_rdrct = check_rdrct(tokens[i], "<", 0, data);
		cmds[i]->output_rdrct = check_rdrct(tokens[i], ">", 0, data);
		cmds[i]->cmd = split_arg(tokens[i], data);
		cmds[i]->infile_fd = -1;
		cmds[i]->outfile_fd = -1;
		if (!cmds[i]->input_rdrct
			|| !cmds[i]->output_rdrct || !cmds[i]->cmd)
			return (false);
	}
	return (true);
}

bool	is_del(char c, char *del, t_quote *quote)
{
	bool	del_quote;

	del_quote = (ft_strchr(del, '"') && ft_strchr(del, '\''));
	if (quote)
	{
		if (c == '"' && *quote == DOUBLE_Q)
			*quote = NONE_Q;
		else if (c == '"' && *quote == NONE_Q)
			*quote = DOUBLE_Q;
		else if (c == '\'' && *quote == SINGLE_Q)
			*quote = NONE_Q;
		else if (c == '\'' && *quote == NONE_Q)
			*quote = SINGLE_Q;
		if (!del_quote && *quote != NONE_Q)
			return (false);
		if ((*quote == SINGLE_Q && c == '"')
			|| (*quote == DOUBLE_Q && c == '\''))
			return (false);
	}
	return (ft_strchr(del, c) != NULL);
}
