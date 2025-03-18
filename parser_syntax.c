/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:24:37 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 02:54:43 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	is_redirect(char *token);
static bool	print_syntax_error(char *token);

bool	check_syntax(t_list *tokens)
{
	t_list	*current;
	char	*prev;
	char	*token;

	if (!tokens)
		return (true);
	prev = NULL;
	current = tokens;
	while (current)
	{
		token = (char *)current->content;
		if ((prev && ft_strncmp(prev, "|", 2) == 0
				&& ft_strncmp(token, "|", 2) == 0)
			|| (prev && is_redirect(prev)
				&& (ft_strncmp(token, "|", 2) == 0 || is_redirect(token))))
			return (print_syntax_error(token));
		prev = token;
		current = current->next;
	}
	if (prev && (ft_strncmp(prev, "|", 2) == 0 || is_redirect(prev)))
		return (print_syntax_error("newline"));
	return (true);
}

static bool	is_redirect(char *token)
{
	return (ft_strncmp(token, "<", 2) == 0 || ft_strncmp(token, ">", 2) == 0
		|| ft_strncmp(token, "<<", 3) == 0 || ft_strncmp(token, ">>", 3) == 0);
}

static bool	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("`\n", 2);
	return (false);
}
