/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 23:55:23 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 23:55:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool			token_to_file(t_redirect **redirects, t_parser data);
static bool			set_heredocument(t_redirect *redirect,
						t_quote quote, char *token);
static char			**set_file_name(char *token, t_parser data);
static t_redirect	**list_to_redirects(t_list *redirect_list);

bool	get_redirects(t_redirect ***redirects, t_list **tokens, t_parser data)
{
	t_list	*redirect_list;

	redirect_list = NULL;
	if (!get_redirect_list(&redirect_list, tokens))
		return (false);
	*redirects = list_to_redirects(redirect_list);
	if (!*redirects)
		return (false);
	if (!token_to_file(*redirects, data))
	{
		free_redirects(*redirects);
		*redirects = NULL;
		return (false);
	}
	return (true);
}

static bool	token_to_file(t_redirect **redirects, t_parser data)
{
	int		i;

	i = 0;
	while (redirects[i])
	{
		if (redirects[i]->type == HEREDOCUMENT
			&& !set_heredocument(redirects[i], NONE_Q, redirects[i]->token))
			return (false);
		else if (redirects[i]->type != HEREDOCUMENT)
			redirects[i]->file = set_file_name(redirects[i]->token, data);
		if (!redirects[i]->file)
			return (false);
		i++;
	}
	return (true);
}

static bool	set_heredocument(t_redirect *redirect, t_quote quote, char *token)
{
	int		i;
	int		heredoc_i;

	i = -1;
	heredoc_i = 0;
	while (token[++i])
		if (is_in_quote(token[i], &quote, false) || !ft_strchr("'\"", token[i]))
			heredoc_i++;
	if (i != heredoc_i)
		redirect->is_quoted = true;
	redirect->file = (char **)ft_calloc(2, sizeof(char *));
	if (redirect->file)
		redirect->file[0] = (char *)ft_calloc(heredoc_i + 1, sizeof(char));
	if (!redirect->file || !redirect->file[0])
	{
		free(redirect->file);
		return (perror_bool("malloc"));
	}
	i = -1;
	heredoc_i = 0;
	while (token[++i])
		if (is_in_quote(token[i], &quote, false) || !ft_strchr("'\"", token[i]))
			redirect->file[0][heredoc_i++] = token[i];
	return (true);
}

static char	**set_file_name(char *token, t_parser data)
{
	t_list	*expanded_list;
	t_list	*tmp;
	int		file_count;
	char	**files;

	expanded_list = NULL;
	if (!expand_env_quote(&expanded_list, token, &data))
		return (NULL);
	file_count = ft_lstsize(expanded_list);
	files = (char **)ft_calloc(file_count + 1, sizeof(char *));
	if (!files)
	{
		ft_lstclear(&expanded_list, free);
		return ((char **)perror_ptr("malloc"));
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

static t_redirect	**list_to_redirects(t_list *redirect_list)
{
	t_redirect	**redirects;
	t_list		*tmp;
	int			redirect_count;

	redirect_count = ft_lstsize(redirect_list);
	redirects = (t_redirect **)ft_calloc(redirect_count + 1,
			sizeof(t_redirect *));
	if (!redirects)
	{
		ft_lstclear(&redirect_list, free_redirect);
		return ((t_redirect **)perror_ptr("malloc"));
	}
	redirect_count = 0;
	while (redirect_list)
	{
		redirects[redirect_count] = (t_redirect *)redirect_list->content;
		tmp = redirect_list;
		redirect_list = redirect_list->next;
		free(tmp);
		redirect_count++;
	}
	return (redirects);
}
