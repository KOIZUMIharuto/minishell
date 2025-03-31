/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 23:55:14 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 23:55:18 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool				get_redirect(t_redirect **redirect, t_list **tokens);
static t_redirect_type	get_redirect_type(char *token);
static bool				set_redirect(t_redirect **redirect,
							t_redirect_type type, char *token);
static void				rejoin_tokens(t_list **tokens,
							t_list **cur, t_list *prev);

bool	get_redirect_list(t_list **redirect_list, t_list **tokens)
{
	t_redirect	*redirect;
	t_list		*redirect_node;

	while (true)
	{
		redirect = NULL;
		if (!get_redirect(&redirect, tokens))
		{
			ft_lstclear(redirect_list, free);
			return (false);
		}
		if (!redirect)
			break ;
		redirect_node = ft_lstnew(redirect);
		if (!redirect_node)
		{
			free_redirect(redirect);
			ft_lstclear(redirect_list, free_redirect);
			return (perror_bool("malloc"));
		}
		ft_lstadd_back(redirect_list, redirect_node);
	}
	return (true);
}

static bool	get_redirect(t_redirect **redirect, t_list **tokens)
{
	t_list			*prev;
	t_list			*cur;
	char			*token;
	t_redirect_type	redirect_type;

	prev = NULL;
	cur = *tokens;
	while (cur)
	{
		token = (char *)cur->content;
		redirect_type = get_redirect_type(token);
		if (redirect_type != NONE_RDRCT)
		{
			token = (char *)cur->next->content;
			rejoin_tokens(tokens, &cur, prev);
			cur->content = NULL;
			rejoin_tokens(tokens, &cur, prev);
			return (set_redirect(redirect, redirect_type, token));
		}
		prev = cur;
		cur = prev->next;
	}
	return (true);
}

static t_redirect_type	get_redirect_type(char *token)
{
	if (token[0] == '<' && token[1] == '\0')
		return (INPUT_RDRCT);
	else if (token[0] == '>' && token[1] == '\0')
		return (OVERWRITE_RDRCT);
	else if (token[0] == '>' && token[1] == '>' && token[2] == '\0')
		return (APPEND_RDRCT);
	else if (token[0] == '<' && token[1] == '<' && token[2] == '\0')
		return (HEREDOCUMENT);
	return (NONE_RDRCT);
}

static bool	set_redirect(t_redirect **redirect,
				t_redirect_type type, char *token)
{
	*redirect = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!*redirect)
	{
		free(token);
		return (perror_bool("malloc"));
	}
	(*redirect)->type = type;
	(*redirect)->token = token;
	(*redirect)->file = NULL;
	(*redirect)->is_quoted = false;
	(*redirect)->fd = -1;
	return (true);
}

static void	rejoin_tokens(t_list **tokens, t_list **cur, t_list *prev)
{
	if (prev)
		prev->next = (*cur)->next;
	else
		*tokens = (*cur)->next;
	ft_lstdelone(*cur, free);
	if (prev)
		*cur = prev->next;
	else
		*cur = *tokens;
}
