/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_delete.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:32:55 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/28 19:11:59 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	hide_pwd(t_env *env_content);

void	env_delete(t_list **env_list, char *key)
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
				hide_pwd(env_content);
			if (prev)
				prev->next = env_list_tmp->next;
			else
				*env_list = env_list_tmp->next;
			ft_lstdelone(env_list_tmp, env_free);
			return ;
		}
		prev = env_list_tmp;
		env_list_tmp = env_list_tmp->next;
	}
}

static void	hide_pwd(t_env *env_content)
{
	env_content->is_hidden = true;
	free(env_content->value);
	env_content->value = NULL;
}
