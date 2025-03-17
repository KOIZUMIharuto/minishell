/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:50:11 by shiori            #+#    #+#             */
/*   Updated: 2025/03/17 00:47:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	env_split(char *env, char **key, char **value)
{
	int		key_len;

	if (!is_valid_key(env))
		return (INVALID);
	key_len = 0;
	while (env[key_len] && env[key_len] != '=')
		key_len++;
	*key = ft_substr(env, 0, key_len);
	if (env[key_len])
		*value = ft_strdup(env + key_len + 1);
	else
		*value = NULL;
	if (!*key || (env[key_len] && !*value))
	{
		free(*key);
		free(*value);
		my_perror("malloc", ENOMEM);
		return (ERROR);
	}
	return (VALID);
}

bool	is_valid_key(char *key)
{
	int	i;

	if (!key || !*key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (print_invalid_key("export", key));
	i = 1;
	while (key[i] && key[i] != '=')
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
