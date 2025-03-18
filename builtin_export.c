/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:44:22 by shiori            #+#    #+#             */
/*   Updated: 2025/03/18 12:19:49 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	export_env(t_list *env_list);

int	builtin_export(char **cmd, t_list *env)
{
	int		i;
	int		status;
	char	*key;
	char	*value;
	t_valid	is_valid;

	if (!cmd[1])
		return (export_env(env));
	i = 0;
	status = 0;
	while (cmd[++i])
	{
		is_valid = env_split(cmd[i], &key, &value);
		if (is_valid == ERROR)
			return (1);
		if (is_valid == INVALID)
		{
			status = 1;
			continue ;
		}
		if (!env_update(&env, key, value))
			status = 1;
	}
	return (status);
}

static int	export_env(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		env_list = env_list->next;
		if (env->is_shell_var || ft_strcmp(env->key, "_") == 0)
			continue ;
		if (printf("declare -x %s", env->key) < 0
			|| (env->value && printf("=\"%s\"", env->value) < 0)
			|| printf("\n") < 0)
			return (perror_int("printf", errno));
	}
	return (0);
}
