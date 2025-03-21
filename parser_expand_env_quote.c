/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_env_quote.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:00:00 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/21 17:53:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	expand_env_quote(t_list **expanded, char *token, t_parser *data)
{
	t_quote	quote;
	char	*expanded_token;
	t_list	*expanded_node;

	data->is_empty_env_exist = false;
	data->tmp = NULL;
	quote = NONE_Q;
	while (token && (*token || (data->tmp && *(data->tmp))))
	{
		expanded_token = recursive_expand(&token, data, quote, 0);
		if (!expanded_token)
		{
			ft_lstclear(expanded, free);
			return (false);
		}
		if (!data->tmp && ft_strlen(expanded_token) == 0
			&& data->is_empty_env_exist)
		{
			free(expanded_token);
			ft_lstclear(expanded, free);
			return (true);
		}
		expanded_node = ft_lstnew(expanded_token);
		if (!expanded_node)
		{
			free(expanded_token);
			ft_lstclear(expanded, free);
			return (perror_bool("malloc", errno));
		}
		ft_lstadd_back(expanded, expanded_node);
	}
	return (true);
}
