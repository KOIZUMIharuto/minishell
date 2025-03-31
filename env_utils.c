/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:50:11 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 23:37:37 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		get_value(char **value, char *key,
					char *line, t_list *env_list);
static t_valid	print_invalid_key(char *cmd, char *key);

t_valid	env_split(char *env, char **key, char **value, t_list *env_list)
{
	t_valid	is_valid;
	int		key_len;

	is_valid = is_valid_key(env);
	if (is_valid != VALID)
		return (is_valid);
	key_len = 0;
	while (env[key_len] && !ft_strchr("+=", env[key_len]))
		key_len++;
	if (env[key_len] == '+' && env[key_len + 1] != '=')
		return (INVALID);
	*key = ft_substr(env, 0, key_len);
	if (!*key)
	{
		perror("malloc");
		return (CRITICAL_ERROR);
	}
	if (!get_value(value, *key, &(env[key_len]), env_list))
		return (CRITICAL_ERROR);
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
		return (perror_bool("malloc"));
	}
	return (true);
}

t_valid	is_valid_key(char *key)
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
	return (VALID);
}

static t_valid	print_invalid_key(char *cmd, char *key)
{
	if (!print_msg("minishell: ", STDERR_FILENO)
		|| !print_msg(cmd, ft_strlen(cmd))
		|| !print_msg(": `", STDERR_FILENO)
		|| !print_msg(key, STDERR_FILENO)
		|| !print_msg("': not a valid identifier\n", STDERR_FILENO))
	{
		g_last_exit_status = -1;
		return (CRITICAL_ERROR);
	}
	return (INVALID);
}
