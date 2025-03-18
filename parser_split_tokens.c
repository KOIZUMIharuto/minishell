/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 02:05:55 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 02:39:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_list	**recursive_split_tokens(t_list *tokens, int cmd_count);

t_list	**split_tokens(t_list *tokens)
{
	t_list	**cmds;

	cmds = recursive_split_tokens(tokens, 0);
	if (!cmds)
		return ((t_list **)perror_ptr("malloc", errno));
	return (cmds);
}

static t_list	**recursive_split_tokens(t_list *tokens, int cmd_count)
{
	t_list	**cmds;
	t_list	*cmd_head;
	t_list	*prev_token;

	if (!tokens)
		return ((t_list **)ft_calloc(cmd_count + 1, sizeof(t_list *)));
	cmd_head = tokens;
	prev_token = NULL;
	while (tokens && ft_strncmp((char *)tokens->content, "|", 2) != 0)
	{
		prev_token = tokens;
		tokens = tokens->next;
	}
	if (tokens)
		tokens = tokens->next;
	cmds = recursive_split_tokens(tokens, cmd_count + 1);
	if (!cmds)
		return (NULL);
	cmds[cmd_count] = cmd_head;
	if (prev_token)
	{
		ft_lstdelone(prev_token->next, free);
		prev_token->next = NULL;
	}
	return (cmds);
}
