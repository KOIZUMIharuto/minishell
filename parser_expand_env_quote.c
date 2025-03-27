/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_env_quote.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:00:00 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/25 12:19:29 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	free_expanded_bool(t_list **expanded, char *token, bool ret);

bool	expand_env_quote(t_list **expanded, char *token, t_parser *data)
{
	char	*expanded_token;
	t_list	*expanded_node;

	while (token && (*token || (data->tmp && *(data->tmp))))
	{
		expanded_token = recursive_expand(&token, data, NONE_Q, 0);
		if (!expanded_token)
			return (free_expanded_bool(expanded, NULL, false));
		if (!data->tmp && !ft_strlen(expanded_token) && data->is_env_empty)
			return (free_expanded_bool(expanded, expanded_token, true));
		expanded_node = ft_lstnew(expanded_token);
		if (!expanded_node)
			return (free_expanded_bool(expanded, expanded_token,
					perror_bool("malloc")));
		ft_lstadd_back(expanded, expanded_node);
		while ((data->tmp && *(data->tmp))
			&& ft_strchr(data->del, *(data->tmp)))
			(data->tmp)++;
		if (data->tmp && !*(data->tmp))
			data->tmp = NULL;
	}
	return (true);
}

static bool	free_expanded_bool(t_list **expanded, char *token, bool ret)
{
	ft_lstclear(expanded, free);
	free(token);
	return (ret);
}
