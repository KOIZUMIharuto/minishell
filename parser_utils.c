/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:19:53 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 21:44:48 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_in_quote(char c, t_quote *quote, bool is_include_quote)
{
	if (*quote == NONE_Q)
	{
		if (c == '\'')
			*quote = SINGLE_Q;
		else if (c == '"')
			*quote = DOUBLE_Q;
		return (ft_strchr("'\"", c) && is_include_quote);
	}
	else if (*quote == SINGLE_Q && c == '\'')
		*quote = NONE_Q;
	else if (*quote == DOUBLE_Q && c == '"')
		*quote = NONE_Q;
	return (*quote != NONE_Q || is_include_quote);
}

bool	is_effective_quote(char c, t_quote *quote)
{
	if (c == '"' && *quote == DOUBLE_Q)
		*quote = NONE_Q;
	else if (c == '"' && *quote == NONE_Q)
		*quote = DOUBLE_Q;
	else if (c == '\'' && *quote == SINGLE_Q)
		*quote = NONE_Q;
	else if (c == '\'' && *quote == NONE_Q)
		*quote = SINGLE_Q;
	if ((*quote == SINGLE_Q && c == '"')
		|| (*quote == DOUBLE_Q && c == '\''))
		return (false);
	return (ft_strchr("'\"", c) != NULL);
}
