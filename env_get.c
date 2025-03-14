/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:27:05 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 00:46:40 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//発見: t_env*, 未発見: NULL
t_env	*env_get(t_list *env_list, char *key)
{
	t_list	*env_list_tmp;
	t_env	*env_content;

	env_list_tmp = env_list;
	while (env_list_tmp)
	{
		env_content = (t_env *)env_list_tmp->content;
		if (ft_strcmp(env_content->key, key) == 0)
			return (env_content);
		env_list_tmp = env_list_tmp->next;
	}
	return (NULL);
}