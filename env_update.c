/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:25:18 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 00:46:40 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		split_env(char *env, char **key, char **value);
static bool		add_env(t_list **env_list, char *key, char *value);
static t_list	*create_new_env(char *key, char *value);

//成功: true, 失敗: false
bool	env_update(t_list **env_list, char *env)
{
	char	*key;
	char	*value;
	t_env	*env_content;

	if (!split_env(env, &key, &value))
		return (false);
	env_content = env_get(*env_list, key);
	if (!env_content)
		return (add_env(env_list, key, value));
	if (!value)
		return (true);
	free(env_content->value);
	env_content->value = value;
	return (true);
}

//成功: true, 失敗: false
static bool	split_env(char *env, char **key, char **value)
{
	int		key_len;

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
		return (false);
	}
	return (true);
}

//成功: true, 失敗: false
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
	t_env	*env_content;

	env_content = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!env_content)
	{
		free(key);
		free(value);
		return (false);
	}
	env_content->key = key;
	env_content->value = value;
	new_env = ft_lstnew((void *)env_content);
	if (!new_env)
	{
		env_free((void *)env_content);
		return (false);
	}
	return (new_env);
}