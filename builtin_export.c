/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:44:22 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:32:17 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	export_env(t_list *env_list);

t_valid	builtin_export(char **cmd, t_list *env)
{
	char	*key;
	char	*value;
	t_valid	is_valid;
	t_valid	valid_status;

	if (!cmd[1])
		return (export_env(env));
	valid_status = VALID;
	while (*(++cmd))
	{
		is_valid = env_split(*cmd, &key, &value, env);
		if (is_valid == ERROR)
			return (is_valid);
		if (is_valid == INVALID)
		{
			valid_status = INVALID;
			continue ;
		}
		if (!env_update(&env, key, value))
		{
			g_last_exit_status = -1;
			valid_status = ERROR;
		}
	}
	return (valid_status);
}

static int	export_env(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		env_list = env_list->next;
		if (env->is_hidden || ft_strcmp(env->key, "_") == 0)
			continue ;
		if (printf("declare -x %s", env->key) < 0
			|| (env->value && printf("=\"%s\"", env->value) < 0)
			|| printf("\n") < 0)
			return (perror_int("printf"));
	}
	return (0);
}
