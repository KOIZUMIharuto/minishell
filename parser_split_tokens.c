/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 02:05:55 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/25 12:44:04 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_list	**recursive_split_tokens(t_list *tokens, int cmd_count);

bool	split_tokens(t_list ***splited_tokens, t_list *tokens)
{
	*splited_tokens = recursive_split_tokens(tokens, 0);
	if (!*splited_tokens)
	{
		ft_lstclear(&tokens, free);
		return (perror_bool("malloc"));
	}
	return (true);
}

static t_list	**recursive_split_tokens(t_list *tokens, int cmd_count)
{
	t_list	**splited_tokens;
	t_list	*cmd_head;
	t_list	*prev_token;

	cmd_head = tokens;
	prev_token = NULL;
	if (!tokens)
		return ((t_list **)ft_calloc(cmd_count + 1, sizeof(t_list *)));
	while (tokens && ft_strncmp((char *)tokens->content, "|", 2) != 0)
	{
		prev_token = tokens;
		tokens = prev_token->next;
	}
	if (tokens)
		tokens = tokens->next;
	splited_tokens = recursive_split_tokens(tokens, cmd_count + 1);
	if (!splited_tokens)
		return (NULL);
	splited_tokens[cmd_count] = cmd_head;
	if (!prev_token->next)
		return (splited_tokens);
	ft_lstdelone(prev_token->next, free);
	prev_token->next = NULL;
	return (splited_tokens);
}
