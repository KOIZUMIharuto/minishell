/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:51:27 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 14:26:59 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	init_shlvl(t_list **env_list);
static bool	shlvl_not_exist(t_list **env_list, char *shlvl_key);
static bool	init_oldpwd(t_list **env_list);

t_list	*env_init(char **env)
{
	t_list	*env_list;
	char	*key;
	char	*value;

	if (!env)
		return (NULL);
	env_list = NULL;
	while (*env)
	{
		if (env_split(*env, &key, &value) != VALID
			|| !env_update(&env_list, key, value))
		{
			ft_lstclear(&env_list, env_free);
			return (NULL);
		}
		env++;
	}
	if (!init_shlvl(&env_list) || !init_oldpwd(&env_list))
	{
		ft_lstclear(&env_list, env_free);
		return (NULL);
	}
	return (env_list);
}

static bool	init_shlvl(t_list **env_list)
{
	t_env	*env;
	int		shlvl;
	char	*shlvl_key;
	char	*shlvl_value;

	env = env_get(*env_list, "SHLVL", false);
	shlvl_key = ft_strdup("SHLVL");
	if (!shlvl_key)
		return (perror_bool("malloc", errno));
	if (!env)
		return (shlvl_not_exist(env_list, shlvl_key));
	shlvl = ft_atoi(env->value);
	if (shlvl < 0)
		shlvl = 0;
	else
		shlvl++;
	shlvl_value = ft_itoa(shlvl);
	if (!shlvl_value)
	{
		free(shlvl_key);
		return (perror_bool("malloc", errno));
	}
	free(env->value);
	env->value = shlvl_value;
	return (true);
}

static bool	shlvl_not_exist(t_list **env_list, char *shlvl_key)
{
	char	*shlvl_value;

	shlvl_value = ft_itoa(1);
	if (!shlvl_value)
	{
		free(shlvl_key);
		return (perror_bool("malloc", errno));
	}
	return (env_update(env_list, shlvl_key, shlvl_value));
}

static bool	init_oldpwd(t_list **env_list)
{
	t_env	*env;
	char	*oldpwd_key;

	env = env_get(*env_list, "OLDPWD", false);
	if (env)
	{
		free(env->value);
		env->value = NULL;
	}
	else
	{
		oldpwd_key = ft_strdup("OLDPWD");
		if (!oldpwd_key)
			return (perror_bool("malloc", errno));
		if (!env_update(env_list, oldpwd_key, NULL))
		{
			free(oldpwd_key);
			return (false);
		}
	}
	return (true);
}