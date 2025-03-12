/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_quote_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:05:58 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/12 17:37:39 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static char	*env_remained(char **arg, char **env, t_quote quote, int len);
static char	*env_not_remained(char **arg, char **env, t_quote quote, int len);
static void	move_arg_pointer(char **arg, char **env, char *tmp, int i);
static int	expand_env(char *arg, char **env);

char	*recursive_expand(char **arg, char **env, t_quote quote, int len)
{
	char	*expanded;

	expanded = NULL;
	if (*env && **env)
		expanded = env_remained(arg, env, quote, len);
	else
		expanded = env_not_remained(arg, env, quote, len);
	return (expanded);
}

static char	*env_remained(char **arg, char **env, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	i = 0;
	while (**env && quote == NONE_QUOTE && ft_strchr(" \t\n", **env))
		(*env)++;
	tmp = *env;
	while (tmp[i] && !(quote == NONE_QUOTE && ft_strchr(" \t\n", tmp[i])))
		i++;
	*env += i;
	if (!tmp[i])
		expanded = recursive_expand(arg, env, quote, len + i);
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

static char	*env_not_remained(char **arg, char **env, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	expanded = NULL;
	tmp = *arg;
	i = 0;
	while (tmp[i] && !is_del(tmp[i], "\"'", &quote)
		&& !(quote != SINGLE_QUOTE && tmp[i] == '$'))
		i++;
	move_arg_pointer(arg, env, tmp, i);
	if (!tmp[i])
		expanded = (char *)ft_calloc(len + i + 1, sizeof(char));
	else if (tmp[i] == '$')
		expanded = recursive_expand(arg, env, quote, len + i);
	else if (tmp[i] == '\'' || tmp[i] == '"')
		expanded = recursive_expand(arg, env, quote, len + i);
	if (expanded)
		while (i--)
			expanded[len + i] = tmp[i];
	return (expanded);
}

static void	move_arg_pointer(char **arg, char **env, char *tmp, int i)
{
	if (!tmp[i])
		*arg += i;
	else if (tmp[i] == '$')
		*arg += expand_env(*arg + i + 1, env) + i + 1;
	else if (tmp[i] == '\'' || tmp[i] == '"')
		*arg += i + 1;
}

static int	expand_env(char *arg, char **env)
{
	int		key_len;
	int		i;

	key_len = 0;
	while (arg[key_len] && (ft_isalnum(arg[key_len]) || arg[key_len] == '_'))
		key_len++;
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(arg, environ[i], key_len) == 0
			&& environ[i][key_len] == '=')
		{
			*env = &(environ[i][key_len + 1]);
			break ;
		}
		i++;
	}
	return (key_len);
}
