/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokenize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:43:17 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/23 10:57:58 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	handle_quote(char *line, int i, int *token_start, t_quote *quote);
static bool	handle_no_q(t_list **tokens, char *line, int *i, int *token_start);
static bool	add_token(t_list **tokens, char *start, int length);

t_list	*tokenize(char *line)
{
	t_list	*tokens;
	int		i;
	int		token_start;
	t_quote	quote;

	tokens = NULL;
	i = 0;
	token_start = -1;
	quote = NONE_Q;
	while (line[i])
	{
		if ((line[i] == '\'' && quote != DOUBLE_Q)
			|| (line[i] == '"' && quote != SINGLE_Q))
			handle_quote(line, i, &token_start, &quote);
		else if (quote == NONE_Q
			&& !handle_no_q(&tokens, line, &i, &token_start))
			return (NULL);
		i++;
	}
	if (token_start != -1
		&& !add_token(&tokens, line + token_start, i - token_start))
		return (NULL);
	return (tokens);
}

static void	handle_quote(char *line, int i, int *token_start, t_quote *quote)
{
	if (line[i] == '\'')
	{
		if (*quote == SINGLE_Q)
			*quote = NONE_Q;
		else
			*quote = SINGLE_Q;
		if (*token_start == -1)
			*token_start = i;
	}
	else if (line[i] == '"')
	{
		if (*quote == DOUBLE_Q)
			*quote = NONE_Q;
		else
			*quote = DOUBLE_Q;
		if (*token_start == -1)
			*token_start = i;
	}
}

static bool	handle_no_q(t_list **tokens, char *line, int *i, int *token_start)
{
	if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
	{
		if (*token_start != -1
			&& !add_token(tokens, line + *token_start, *i - *token_start))
			return (false);
		*token_start = -1;
		if ((line[*i] == '<' && line[*i + 1] == '<')
			|| (line[*i] == '>' && line[*i + 1] == '>'))
			return (add_token(tokens, line + (*i)++, 2));
		else
			return (add_token(tokens, line + *i, 1));
	}
	else if (ft_strchr(" \t\n", line[*i]))
	{
		if (*token_start != -1
			&& !add_token(tokens, line + *token_start, *i - *token_start))
			return (false);
		*token_start = -1;
	}
	else if (*token_start == -1)
		*token_start = *i;
	return (true);
}

static bool	add_token(t_list **tokens, char *start, int length)
{
	t_list	*new_token;
	char	*token_str;

	if (length <= 0)
		return (true);
	token_str = ft_substr(start, 0, length);
	if (!token_str)
		return (perror_bool("malloc", errno));
	new_token = ft_lstnew(token_str);
	if (!new_token)
	{
		free(token_str);
		ft_lstclear(tokens, free);
		return (perror_bool("malloc", errno));
	}
	ft_lstadd_back(tokens, new_token);
	return (true);
}
