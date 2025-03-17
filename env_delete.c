/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_delete.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:32:55 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 14:32:29 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	delete_pwd(t_env *env_content);

bool	env_delete(t_list **env_list, char *key)
{
	t_list	*env_list_tmp;
	t_list	*prev;
	t_env	*env_content;

	env_list_tmp = *env_list;
	prev = NULL;
	while (env_list_tmp)
	{
		env_content = (t_env *)env_list_tmp->content;
		if (ft_strcmp(env_content->key, key) == 0)
		{
			if (ft_strcmp(key, "PWD") == 0)
				return (delete_pwd(env_content));
			if (prev)
				prev->next = env_list_tmp->next;
			else
				*env_list = env_list_tmp->next;
			ft_lstdelone(env_list_tmp, env_free);
			return (true);
		}
		prev = env_list_tmp;
		env_list_tmp = env_list_tmp->next;
	}
	return (true);
}

static bool	delete_pwd(t_env *env_content)
{
	if (ft_strcmp(env_content->key, "PWD") != 0)
		return (true);
	env_content->is_shell_var = true;
	free(env_content->value);
	env_content->value = ft_strdup("");
	if (!env_content->value)
		return (perror_bool("malloc", errno));
	return (true);
}
