/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:25:18 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/23 10:58:10 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		add_env(t_list **env_list, char *key, char *value);
static t_list	*create_new_env(char *key, char *value);

bool	env_update(t_list **env_list, char *key, char *value)
{
	t_env	*env;

	env = env_get(*env_list, key, true);
	if (!env)
		return (add_env(env_list, key, value));
	free(key);
	if (env->is_hidden || value)
	{
		free(env->value);
		env->value = value;
	}
	env->is_hidden = false;
	return (true);
}

static bool	add_env(t_list **env_list, char *key, char *value)
{
	t_list	*new_list;
	t_list	*env_list_tmp;
	t_env	*env_tmp;

	new_list = create_new_env(key, value);
	if (!new_list)
		return (false);
	if (!*env_list || ft_strcmp(key, ((t_env *)(*env_list)->content)->key) < 0)
	{
		new_list->next = *env_list;
		*env_list = new_list;
		return (true);
	}
	env_list_tmp = *env_list;
	while (env_list_tmp->next)
	{
		env_tmp = (t_env *)env_list_tmp->next->content;
		if (ft_strcmp(key, env_tmp->key) < 0)
			break ;
		env_list_tmp = env_list_tmp->next;
	}
	new_list->next = env_list_tmp->next;
	env_list_tmp->next = new_list;
	return (true);
}

static t_list	*create_new_env(char *key, char *value)
{
	t_list	*new_env;
	t_env	*env;

	env = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!env)
	{
		free(key);
		free(value);
		return ((t_list *)perror_ptr("malloc", ENOMEM));
	}
	env->key = key;
	env->value = value;
	env->is_hidden = false;
	new_env = ft_lstnew((void *)env);
	if (!new_env)
	{
		env_free((void *)env);
		return ((t_list *)perror_ptr("malloc", ENOMEM));
	}
	return (new_env);
}
