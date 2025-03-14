/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_quote_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:05:58 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/14 15:18:57 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static char	*env_remained(char **arg, t_data *data, t_quote quote, int len);
static char	*env_not_remained(char **arg, t_data *data, t_quote quote, int len);
static void	move_arg_pointer(char **arg, t_data *data, char *tmp, int i);
static int	expand_env(char *arg, t_data *data);

char	*recursive_expand(char **arg, t_data *data, t_quote quote, int len)
{
	char	*expanded;

	expanded = NULL;
	if (data->tmp && *(data->tmp))
		expanded = env_remained(arg, data, quote, len);
	else
		expanded = env_not_remained(arg, data, quote, len);
	return (expanded);
}

static char	*env_remained(char **arg, t_data *data, t_quote quote, int len)
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

static char	*env_not_remained(char **arg, t_data *data, t_quote quote, int len)
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

static void	move_arg_pointer(char **arg, t_data *data, char *tmp, int i)
{
	if (!tmp[i])
		*arg += i;
	else if (tmp[i] == '$')
		*arg += expand_env(*arg + i + 1, data) + i + 1;
	else if (tmp[i] == '\'' || tmp[i] == '"')
		*arg += i + 1;
}

static int	expand_env(char *arg, t_data *data)
{
	int		key_len;
	int		i;

	if (arg[0] == '?')
	{
		data->tmp = data->exit_status;
		return (1);
	}
	key_len = 0;
	while (arg[key_len] && (ft_isalnum(arg[key_len]) || arg[key_len] == '_'))
		key_len++;
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(arg, data->env[i], key_len) == 0
			&& data->env[i][key_len] == '=')
		{
			data->tmp = &(data->env[i][key_len + 1]);
			break ;
		}
		i++;
	}
	return (key_len);
}
