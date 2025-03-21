/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:51:27 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/21 15:34:23 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	init_shlvl(t_list **env_list);
static bool	shlvl_not_exist(t_list **env_list);
static bool	init_oldpwd(t_list **env_list);
static bool	init_pwd(t_list **env_list);

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
	if (!init_shlvl(&env_list) || !init_oldpwd(&env_list)
		|| !init_pwd(&env_list))
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
	char	*shlvl_value;

	env = env_get(*env_list, "SHLVL", false);
	if (!env)
		return (shlvl_not_exist(env_list));
	shlvl = ft_atoi(env->value);
	if (shlvl < 0)
		shlvl = 0;
	else
		shlvl++;
	shlvl_value = ft_itoa(shlvl);
	if (!shlvl_value)
		return (perror_bool("malloc", errno));
	free(env->value);
	env->value = shlvl_value;
	return (true);
}

static bool	shlvl_not_exist(t_list **env_list)
{
	char	*shlvl_key;
	char	*shlvl_value;

	shlvl_key = ft_strdup("SHLVL");
	shlvl_value = ft_itoa(1);
	if (!shlvl_key || !shlvl_value)
	{
		free(shlvl_key);
		free(shlvl_value);
		return (perror_bool("malloc", errno));
	}
	return (env_update(env_list, shlvl_key, shlvl_value));
}

static bool	init_oldpwd(t_list **env_list)
{
	t_env		*env;
	struct stat	path_stat;
	char		*oldpwd_key;

	env = env_get(*env_list, "OLDPWD", false);
	if (env && env->value
		&& (stat(env->value, &path_stat) || !S_ISDIR(path_stat.st_mode)))
		(void)env_delete(env_list, "OLDPWD");
	else if (!env)
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

static bool	init_pwd(t_list **env_list)
{
	t_env	*env;
	char	*pwd_key;
	char	*pwd_value;

	pwd_value = get_pwd();
	if (!pwd_value)
		return (false);
	env = env_get(*env_list, "PWD", false);
	if (env)
	{
		free(env->value);
		env->value = pwd_value;
		return (true);
	}
	pwd_key = ft_strdup("PWD");
	if (!pwd_key)
	{
		free(pwd_value);
		return (perror_bool("malloc", errno));
	}
	return (env_update(env_list, pwd_key, pwd_value));
}
