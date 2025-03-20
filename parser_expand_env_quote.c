/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_env_quote.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:00:00 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/20 15:00:48 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*expand_env_quote(char *token, t_parser *data)
{
	t_list	*expanded_list;
	t_quote	quote;
	char	*expanded_token;
	t_list	*expanded_node;

	data->is_empty_env_exist = false;
	data->tmp = NULL;
	expanded_list = NULL;
	quote = NONE_Q;
	while (token && (*token || (data->tmp && *(data->tmp))))
	{
		expanded_token = recursive_expand(&token, data, quote, 0);
		if (!expanded_token)
		{
			ft_lstclear(&expanded_list, free);
			return (NULL);
		}
		if (!data->tmp && ft_strlen(expanded_token) == 0
			&& data->is_empty_env_exist)
		{
			free(expanded_token);
			ft_lstclear(&expanded_list, free);
			return (NULL);
		}
		expanded_node = ft_lstnew(expanded_token);
		if (!expanded_node)
		{
			free(expanded_token);
			ft_lstclear(&expanded_list, free);
			return ((t_list *)perror_ptr("malloc", errno));
		}
		ft_lstadd_back(&expanded_list, expanded_node);
	}
	return (expanded_list);
}
