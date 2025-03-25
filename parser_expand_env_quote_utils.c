/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_env_quote_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:43:48 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/25 12:19:29 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*env_remain(char **token, t_parser *data, t_quote quote, int len);
static char	*noenv_remain(char **token, t_parser *data, t_quote quote, int len);
static void	move_arg_pointer(char **token, t_parser *data, char *tmp, int i);
static void	expand_env(char *token, int key_len, t_parser *data);

char	*recursive_expand(char **token, t_parser *data, t_quote quote, int len)
{
	char	*expanded;

	expanded = NULL;
	if (data->tmp && *(data->tmp))
		expanded = env_remain(token, data, quote, len);
	else
		expanded = noenv_remain(token, data, quote, len);
	if (!expanded)
		return ((char *)perror_ptr("malloc"));
	return (expanded);
}

static char	*env_remain(char **token, t_parser *data, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	i = 0;
	while (*(data->tmp) && quote == NONE_Q
		&& ft_strchr(data->del, *(data->tmp)))
		(data->tmp)++;
	tmp = data->tmp;
	while (tmp[i] && !(quote == NONE_Q && ft_strchr(data->del, tmp[i])))
		i++;
	data->tmp += i;
	if (!tmp[i])
		expanded = recursive_expand(token, data, quote, len + i);
	else
	{
		expanded = (char *)malloc(len + i + 1);
		if (expanded)
			expanded[len + i] = '\0';
	}
	if (!expanded)
		return (NULL);
	while (i--)
		expanded[len + i] = tmp[i];
	return (expanded);
}

static char	*noenv_remain(char **token, t_parser *data, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	expanded = NULL;
	tmp = *token;
	i = 0;
	while (tmp[i] && !is_effective_quote(tmp[i], &quote)
		&& !(quote != SINGLE_Q && tmp[i] == '$' && (tmp[i + 1]
				&& (ft_isalnum(tmp[i + 1]) || ft_strchr("_?", tmp[i + 1])))))
		i++;
	move_arg_pointer(token, data, tmp, i);
	if (!tmp[i])
		expanded = (char *)ft_calloc(len + i + 1, sizeof(char));
	else if (tmp[i] == '$')
		expanded = recursive_expand(token, data, quote, len + i);
	else if (tmp[i] == '\'' || tmp[i] == '"')
		expanded = recursive_expand(token, data, quote, len + i);
	if (!expanded)
		return (NULL);
	while (i--)
		expanded[len + i] = tmp[i];
	return (expanded);
}

static void	move_arg_pointer(char **token, t_parser *data, char *tmp, int i)
{
	int		key_len;
	char	*key_tmp;

	if (!tmp[i])
		*token += i;
	else if (tmp[i] == '$')
	{
		key_len = 0;
		key_tmp = tmp + i + 1;
		if (key_tmp[key_len] == '?')
			key_len++;
		else if (ft_isalpha(key_tmp[key_len]) || key_tmp[key_len] == '_')
			while (ft_isalnum(key_tmp[key_len]) || key_tmp[key_len] == '_')
				key_len++;
		expand_env(*token + i + 1, key_len, data);
		if (!data->tmp)
		{
			data->is_env_empty = true;
			data->is_failed = false;
		}
		*token += key_len + i + 1;
	}
	else if (tmp[i] == '\'' || tmp[i] == '"')
		*token += i + 1;
}

static void	expand_env(char *token, int key_len, t_parser *data)
{
	t_list	*list_tmp;
	t_env	*env;

	if (token[0] == '?')
	{
		data->tmp = data->exit_status;
		return ;
	}
	list_tmp = data->env;
	data->tmp = NULL;
	while (list_tmp)
	{
		env = (t_env *)list_tmp->content;
		list_tmp = list_tmp->next;
		if (env->is_hidden)
			continue ;
		if (ft_strncmp(env->key, token, key_len) == 0 && !env->key[key_len])
		{
			data->tmp = env->value;
			return ;
		}
	}
}
