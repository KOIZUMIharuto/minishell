/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdrct_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:54:00 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 21:24:20 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool			get_rdrct(t_rdrct **rdrct, t_list **tokens);
static t_rdrct_type	get_rdrct_type(char *token);
static bool			set_rdrct(t_rdrct **rdrct, t_rdrct_type type, char *token);
static void			rejoin_tokens(t_list **tokens, t_list **cur, t_list *prev);

bool	get_rdrct_list(t_list **rdrct_list, t_list **tokens)
{
	t_rdrct	*rdrct;
	t_list	*rdrct_node;

	while (true)
	{
		rdrct = NULL;
		if (!get_rdrct(&rdrct, tokens))
		{
			ft_lstclear(rdrct_list, free);
			return (false);
		}
		if (!rdrct)
			break ;
		rdrct_node = ft_lstnew(rdrct);
		if (!rdrct_node)
		{
			free_rdrct(rdrct);
			ft_lstclear(rdrct_list, free_rdrct);
			return (perror_bool("malloc"));
		}
		ft_lstadd_back(rdrct_list, rdrct_node);
	}
	return (true);
}

static bool	get_rdrct(t_rdrct **rdrct, t_list **tokens)
{
	t_list			*prev;
	t_list			*cur;
	char			*token;
	t_rdrct_type	rdrct_type;

	prev = NULL;
	cur = *tokens;
	while (cur)
	{
		token = (char *)cur->content;
		rdrct_type = get_rdrct_type(token);
		if (rdrct_type != NONE_RDRCT)
		{
			token = (char *)cur->next->content;
			rejoin_tokens(tokens, &cur, prev);
			cur->content = NULL;
			rejoin_tokens(tokens, &cur, prev);
			return (set_rdrct(rdrct, rdrct_type, token));
		}
		prev = cur;
		cur = prev->next;
	}
	return (true);
}

static t_rdrct_type	get_rdrct_type(char *token)
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

static bool	set_rdrct(t_rdrct **rdrct, t_rdrct_type type, char *token)
{
	*rdrct = (t_rdrct *)ft_calloc(1, sizeof(t_rdrct));
	if (!*rdrct)
	{
		free(token);
		return (perror_bool("malloc"));
	}
	(*rdrct)->type = type;
	(*rdrct)->token = token;
	(*rdrct)->file = NULL;
	(*rdrct)->is_quoted = false;
	(*rdrct)->fd = -1;
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
