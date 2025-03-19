/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rdrct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:02:47 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 21:49:55 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		token_to_file(t_rdrct **rdrcts, t_parser data);
static char		**set_heredocument(char *token);
static char		**set_file_name(char *token, t_parser data);
static t_rdrct	**list_to_rdrcts(t_list *rdrct_list);

bool	get_rdrcts(t_rdrct ***rdrcts, t_list **tokens, char key, t_parser data)
{
	t_list	*rdrct_list;

	rdrct_list = NULL;
	if (!get_rdrct_list(&rdrct_list, tokens, key))
		return (false);
	*rdrcts = list_to_rdrcts(rdrct_list);
	if (!*rdrcts)
		return (false);
	if (!token_to_file(*rdrcts, data))
	{
		free_rdrcts(*rdrcts);
		*rdrcts = NULL;
		return (false);
	}
	return (true);
}

static bool	token_to_file(t_rdrct **rdrcts, t_parser data)
{
	int		i;

	i = 0;
	while (rdrcts[i])
	{
		if (rdrcts[i]->type == HEREDOCUMENT)
			rdrcts[i]->file = set_heredocument(rdrcts[i]->token);
		else
			rdrcts[i]->file = set_file_name(rdrcts[i]->token, data);
		if (!rdrcts[i]->file)
			return (false);
		i++;
	}
	return (true);
}

static char	**set_heredocument(char *token)
{
	t_quote	quote;
	int		i;
	int		heredoc_i;
	char	**heredoc;

	quote = NONE_Q;
	i = -1;
	heredoc_i = 0;
	while (token[++i])
		if (is_in_quote(token[i], &quote, false) || !ft_strchr("'\"", token[i]))
			heredoc_i++;
	heredoc = (char **)ft_calloc(2, sizeof(char *));
	if (heredoc)
		heredoc[0] = (char *)ft_calloc(heredoc_i + 1, sizeof(char));
	if (!heredoc || !heredoc[0])
	{
		free(heredoc);
		return (perror_ptr("malloc", errno));
	}
	i = -1;
	heredoc_i = 0;
	while (token[++i])
		if (is_in_quote(token[i], &quote, false) || !ft_strchr("'\"", token[i]))
			heredoc[0][heredoc_i++] = token[i];
	return (heredoc);
}

static char	**set_file_name(char *token, t_parser data)
{
	t_list	*expanded_list;
	t_list	*tmp;
	int		file_count;
	char	**files;

	expanded_list = expand_env_quote(token, data);
	if (!expanded_list)
		return (NULL);
	file_count = ft_lstsize(expanded_list);
	files = (char **)ft_calloc(file_count + 1, sizeof(char *));
	if (!files)
	{
		ft_lstclear(&expanded_list, free);
		return ((char **)perror_ptr("malloc", errno));
	}
	file_count = -1;
	while (expanded_list)
	{
		files[++file_count] = (char *)expanded_list->content;
		tmp = expanded_list;
		expanded_list = expanded_list->next;
		free(tmp);
	}
	return (files);
}

static t_rdrct	**list_to_rdrcts(t_list *rdrct_list)
{
	t_rdrct	**rdrcts;
	t_list	*tmp;
	int		rdrct_count;

	rdrct_count = ft_lstsize(rdrct_list);
	rdrcts = (t_rdrct **)ft_calloc(rdrct_count + 1, sizeof(t_rdrct *));
	if (!rdrcts)
	{
		ft_lstclear(&rdrct_list, free_rdrct);
		return ((t_rdrct **)perror_ptr("malloc", errno));
	}
	rdrct_count = 0;
	while (rdrct_list)
	{
		rdrcts[rdrct_count] = (t_rdrct *)rdrct_list->content;
		tmp = rdrct_list;
		rdrct_list = rdrct_list->next;
		free(tmp);
		rdrct_count++;
	}
	return (rdrcts);
}
