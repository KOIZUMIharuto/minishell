/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:50:11 by shiori            #+#    #+#             */
/*   Updated: 2025/03/25 12:19:29 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	get_value(char **value, char *key, char *line, t_list *env_list);

t_valid	env_split(char *env, char **key, char **value, t_list *env_list)
{
	int		key_len;

	if (!is_valid_key(env))
		return (INVALID);
	key_len = 0;
	while (env[key_len] && !ft_strchr("+=", env[key_len]))
		key_len++;
	if (env[key_len] == '+' && env[key_len + 1] != '=')
		return (INVALID);
	*key = ft_substr(env, 0, key_len);
	if (!*key)
	{
		perror("malloc");
		return (ERROR);
	}
	if (!get_value(value, *key, &(env[key_len]), env_list))
		return (ERROR);
	return (VALID);
}

static bool	get_value(char **value, char *key, char *line, t_list *env_list)
{
	t_env	*env;
	char	*old_value;

	if (!*line)
	{
		*value = NULL;
		return (true);
	}
	old_value = NULL;
	if (*line == '+')
	{
		env = env_get(env_list, key, true);
		if (env)
			old_value = env->value;
		line++;
	}
	line++;
	*value = ft_strjoin(old_value, line);
	if (!*value)
	{
		free(key);
		return (perror_bool("maloc"));
	}
	return (true);
}

bool	is_valid_key(char *key)
{
	int	i;

	if (!key || !*key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (print_invalid_key("export", key));
	i = 1;
	while (key[i] && !ft_strchr("+=", key[i]))
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (print_invalid_key("export", key));
		i++;
	}
	return (true);
}

bool	print_invalid_key(char *cmd, char *key)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}
