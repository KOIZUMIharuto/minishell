/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/18 17:46:59 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdbool.h>

typedef struct s_quote
{
	bool	single_quote;
	bool	double_quote;
}	t_quote;

static char	**recursive_tokenize(char *line, int word_cnt);
static bool	is_whitespace(char c, t_quote *quote);

char	**tokenize_line(char *line)
{
	while (is_whitespace(*line, NULL))
		line++;
	return (recursive_tokenize(line, 0));
}

static char	**recursive_tokenize(char *line, int word_cnt)
{
	char	**tokens;
	char	*token;
	t_quote	quote;
	int		i;

	if (!line || !*line)
	{
		tokens = (char **)ft_calloc(word_cnt + 1, sizeof(char *));
	}
	else
	{
		quote = (t_quote){false, false};
		i = 0;
		while (line[i] && !is_whitespace(line[i], &quote))
			i++;
		token = ft_substr(line, 0, i);
		if (!token)
			return (NULL);
		while (line[i] && is_whitespace(line[i], &quote))
			i++;
		tokens = recursive_tokenize(line + i, word_cnt + 1);
		if (!tokens)
			free(token);
		else
			tokens[word_cnt] = token;
	}
	return (tokens);
}

static bool	is_whitespace(char c, t_quote *quote)
{
	if (quote)
	{
		if (c == '"')
			quote->double_quote = !quote->double_quote;
		else if (c == '\'')
			quote->single_quote = !quote->single_quote;
		if (quote->single_quote || quote->double_quote)
			return (false);
	}
	return (c == ' ' || c <= '\t' || c == '\n');
}

int main()
{
	char *line;
	char **tokens;
	
	while (1)
	{
		write(1, "minishell$ ", 11);
		line = get_next_line(0);
		if (!line)
			break;
		tokens = tokenize_line(line);
		if (!tokens)
		{
			free(line);
			break;
		}
		for (int i = 0; tokens[i]; i++)
		{
			printf("token[%d]: %s\n", i, tokens[i]);
			free(tokens[i]);
		}
		free(tokens);
		free(line);
	}
	return (0);
}