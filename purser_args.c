/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:21:02 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/02/05 15:49:58 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static t_list	*purse_to_list(char *line);
static bool		handle_quotes_and_env(t_list **args, char *arg);
static char		*recursive_expand_quote_and_env(char **arg, char **env, t_quote quote, int len);
static int		expand_env(char *arg, char **env);

char	**split_arg(char *line)
{
	t_list	*args;
	t_list	*tmp;
	int		i;
	char	**cmd;

	if (!line)
		return (NULL);
	args = purse_to_list(line);
	if (args)
		cmd = (char **)malloc(sizeof(char *) * (ft_lstsize(args) + 1));
	if (!args || !cmd)
	{
		ft_lstclear(&args, free);
		return (NULL);
	}
	i = 0;
	while (args)
	{
		tmp = args;
		cmd[i++] = args->content;
		args = args->next;
		free(tmp);
	}
	cmd[i] = NULL;
	return (cmd);
}

static t_list	*purse_to_list(char *line)
{
	t_list	*args;
	t_quote	quote;
	int		i;

	if (!line)
		return (NULL);
	args = NULL;
	while (*line)
	{
		while (*line && is_del(*line, " \t\n", NULL))
			line++;
		if (!*line)
			break ;
		i = 0;
		quote = NONE_QUOTE;
		while (line[i] && !is_del(line[i], " \t\n", &quote))
			i++;
		if (!handle_quotes_and_env(&args, ft_substr(line, 0, i)))
		{
			ft_lstclear(&args, free);
			return (NULL);
		}
		line += i;
	}
	return (args);
}

static bool	handle_quotes_and_env(t_list **args, char *arg)
{
	char	*arg_head;
	t_list	*tmp;
	char	*arg_tmp;
	char	*env;

	arg_head = arg;
	tmp = NULL;
	env = NULL;
	while (arg && (*arg || (env && *env)))
	{
		arg_tmp = recursive_expand_quote_and_env(&arg, &env, NONE_QUOTE, 0);
		if (!arg_tmp)
			break ;
		tmp = ft_lstnew(arg_tmp);
		if (!tmp)
		{
			free (arg_tmp);
			break ;
		}
		ft_lstadd_back(args, tmp);
	}
	free(arg_head);
	if (!arg || !arg_tmp || !tmp)
		return (false);
	return (true);
}

static char	*recursive_expand_quote_and_env(char **arg, char **env, t_quote quote, int len)
{
	char	*expanded;
	char	*tmp;
	int		i;

	expanded = NULL;
	i = 0;
	if (*env && **env)
	{
		while (**env && quote == NONE_QUOTE && ft_strchr(" \t\n", **env))
			(*env)++;
		tmp = *env;
		while (tmp[i] && !(quote == NONE_QUOTE && ft_strchr(" \t\n", tmp[i])))
			i++;
		*env += i;
		if (!tmp[i])
		{
			expanded = recursive_expand_quote_and_env(arg, env, quote, len + i);
			if (!expanded)
				return (NULL);
		}
		else
		{
			expanded = (char *)malloc(len + i + 1);
			if (!expanded)
				return (NULL);
			expanded[len + i] = '\0';
		}
		while (i--)
			expanded[len + i] = tmp[i];
	}
	else
	{
		tmp = *arg;
		while (tmp[i] && !is_del(tmp[i], "\"'", &quote)
			&& !(quote != SINGLE_QUOTE && tmp[i] == '$'))
			i++;
		if (!tmp[i])
		{
			*arg += i;
			expanded = (char *)malloc(len + i + 1);
			if (!expanded)
				return (NULL);
			expanded[len + i] = '\0';
		}
		else if (tmp[i] == '$')
		{
			*arg += expand_env(*arg + i + 1, env) + i + 1;
			expanded = recursive_expand_quote_and_env(arg, env, quote, len + i);
		}
		else if (tmp[i] == '\'' || tmp[i] == '"')
		{
			*arg += i + 1;
			expanded = recursive_expand_quote_and_env(arg, env, quote, len + i);
		}
		if (!expanded)
			return (NULL);
		while (i--)
			expanded[len + i] = tmp[i];
	}
	return (expanded);
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