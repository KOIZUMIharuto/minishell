/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_quote_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:05:58 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/18 15:55:32 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*env_remain(char **arg, t_parser *data, t_quote quote, int len);
static char	*env_not_remain(char **arg, t_parser *data, t_quote quote, int len);
static void	move_arg_pointer(char **arg, t_parser *data, char *tmp, int i);
static void	expand_env(char *arg, int key_len, t_parser *data);

char	*recursive_expand(char **arg, t_parser *data, t_quote quote, int len)
{
	char	*expanded;

	expanded = NULL;
	if (data->tmp && *(data->tmp))
		expanded = env_remain(arg, data, quote, len);
	else
		expanded = env_not_remain(arg, data, quote, len);
	return (expanded);
}

static char	*env_remain(char **arg, t_parser *data, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	i = 0;
	while (*(data->tmp) && quote == NONE_Q && ft_strchr(" \t\n", *(data->tmp)))
		(data->tmp)++;
	tmp = data->tmp;
	while (tmp[i] && !(quote == NONE_Q && ft_strchr(" \t\n", tmp[i])))
		i++;
	data->tmp += i;
	if (!tmp[i])
		expanded = recursive_expand(arg, data, quote, len + i);
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

static char	*env_not_remain(char **arg, t_parser *data, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	expanded = NULL;
	tmp = *arg;
	i = 0;
	while (tmp[i] && !is_del(tmp[i], "\"'", &quote)
		&& !(quote != SINGLE_Q && tmp[i] == '$'))
		i++;
	move_arg_pointer(arg, data, tmp, i);
	if (!tmp[i])
		expanded = (char *)ft_calloc(len + i + 1, sizeof(char));
	else if (tmp[i] == '$')
		expanded = recursive_expand(arg, data, quote, len + i);
	else if (tmp[i] == '\'' || tmp[i] == '"')
		expanded = recursive_expand(arg, data, quote, len + i);
	if (expanded)
		while (i--)
			expanded[len + i] = tmp[i];
	return (expanded);
}

static void	move_arg_pointer(char **arg, t_parser *data, char *tmp, int i)
{
	int		key_len;
	char	*key_tmp;

	if (!tmp[i])
		*arg += i;
	else if (tmp[i] == '$')
	{
		key_len = 0;
		key_tmp = tmp + i + 1;
		if (key_tmp[key_len] == '?')
			key_len++;
		else if (ft_isalpha(key_tmp[key_len]) || key_tmp[key_len] == '_')
			while (ft_isalnum(key_tmp[key_len]) || key_tmp[key_len] == '_')
				key_len++;
		expand_env(*arg + i + 1, key_len, data);
		*arg += key_len + i + 1;
	}
	else if (tmp[i] == '\'' || tmp[i] == '"')
		*arg += i + 1;
}

static void	expand_env(char *arg, int key_len, t_parser *data)
{
	t_list	*list_tmp;
	t_env	*env;

	if (arg[0] == '?')
	{
		data->tmp = data->exit_status;
		return ;
	}
	list_tmp = data->env;
	data->tmp = NULL;
	while (list_tmp)
	{
		env = (t_env *)list_tmp->content;
		if (ft_strncmp(env->key, arg, key_len) == 0 && !env->key[key_len])
		{
			data->tmp = env->value;
			return ;
		}
		list_tmp = list_tmp->next;
	}
}
