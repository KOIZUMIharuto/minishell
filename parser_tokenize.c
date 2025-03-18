/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokenize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:43:17 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/18 23:13:14 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	add_token(t_list **tokens, char *start, int length);

t_list	*tokenize(char *line)
{
	t_list	*tokens = NULL;
	char	*ptr = line;
	char	*token_start = NULL;
	t_quote	quote = NONE_Q;

	while (*ptr)
	{
		// クォートの開始または終了
		if (*ptr == '\'' && quote != DOUBLE_Q)
		{
			quote = (quote == SINGLE_Q) ? NONE_Q : SINGLE_Q;
			if (!token_start) token_start = ptr;  // トークン開始
		}
		else if (*ptr == '"' && quote != SINGLE_Q)
		{
			quote = (quote == DOUBLE_Q) ? NONE_Q : DOUBLE_Q;
			if (!token_start) token_start = ptr;  // トークン開始
		}
		else if (quote == NONE_Q)
		{
			// |, <, >, <<, >> の処理
			if (*ptr == '|' || *ptr == '<' || *ptr == '>')
			{
				if (token_start)
				{
					if (!add_token(&tokens, token_start, ptr - token_start))
						return NULL;
					token_start = NULL;
				}
				if ((ptr[0] == '<' && ptr[1] == '<') || (ptr[0] == '>' && ptr[1] == '>'))
				{
					if (!add_token(&tokens, ptr, 2))
						return NULL;
					ptr++;
				}
				else
					if (!add_token(&tokens, ptr, 1))
						return NULL;
			}
			// 空白文字（トークンの区切り）
			else if (isspace(*ptr))
			{
				if (token_start)
				{
					if (!add_token(&tokens, token_start, ptr - token_start))
						return NULL;
					token_start = NULL;
				}
			}
			// 通常の文字列
			else if (!token_start)
				token_start = ptr;
		}
		ptr++;
	}
	// 最後のトークンを追加
	if (token_start)
		if (!add_token(&tokens, token_start, ptr - token_start))
			return NULL;
	return tokens;
}

// トークンをリストに追加する
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
		return (perror_bool("malloc", errno));
	}
	ft_lstadd_back(tokens, new_token);
	return (true);
}
