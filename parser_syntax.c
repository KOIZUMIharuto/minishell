/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:24:37 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/20 10:23:52 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_quote_closed(char *token);
static bool	is_rdrct(char *token);
static bool	print_syntax_error(char *token);

bool	check_syntax(t_list *tokens)
{
	t_list	*cur;
	char	*prev;
	char	*token;

	if (!tokens)
		return (true);
	prev = NULL;
	cur = tokens;
	while (cur)
	{
		token = (char *)cur->content;
		if (!is_quote_closed(token))
			return (print_syntax_error(token));
		if ((!prev && !ft_strncmp(token, "|", 2))
			|| (prev && !ft_strncmp(prev, "|", 2) && !ft_strncmp(token, "|", 2))
			|| (prev && is_rdrct(prev)
				&& (!ft_strncmp(token, "|", 2) || is_rdrct(token))))
			return (print_syntax_error(token));
		prev = token;
		cur = cur->next;
	}
	if (prev && (ft_strncmp(prev, "|", 2) == 0 || is_rdrct(prev)))
		return (print_syntax_error("newline"));
	return (true);
}

static bool	is_quote_closed(char *token)
{
	int	quote;

	quote = NONE_Q;
	while (*token)
	{
		if (*token == '\'' && quote == NONE_Q)
			quote = SINGLE_Q;
		else if (*token == '\'' && quote == SINGLE_Q)
			quote = NONE_Q;
		else if (*token == '"' && quote == NONE_Q)
			quote = DOUBLE_Q;
		else if (*token == '"' && quote == DOUBLE_Q)
			quote = NONE_Q;
		token++;
	}
	return (quote == NONE_Q);
}

static bool	is_rdrct(char *token)
{
	return (ft_strncmp(token, "<", 2) == 0 || ft_strncmp(token, ">", 2) == 0
		|| ft_strncmp(token, "<<", 3) == 0 || ft_strncmp(token, ">>", 3) == 0);
}

static bool	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	g_last_exit_status = 258;
	return (false);
}
