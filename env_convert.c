/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:51:27 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 00:46:40 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool		split_env(char *env, char **key, char **value);
static bool		add_env(t_list **env_list, char *key, char *value);
static t_list	*create_new_env(char *key, char *value);

t_list	*env_convert(char **env)
{
	t_list	*env_list;

	if (!env)
		return (NULL);
	env_list = NULL;
	while (*env)
	{
		if (!env_update(&env_list, *env))
		{
			ft_lstclear(&env_list, env_free);
			return (NULL);
		}
		env++;
	}
	return (env_list);
}

// //成功: 0, 失敗: 1
// int	print_env(t_list *env_list)
// {
// 	t_list	*env_list_tmp;
// 	t_env	*env_content;

// 	env_list_tmp = env_list;
// 	while (env_list_tmp)
// 	{
// 		env_content = (t_env *)env_list_tmp->content;
// 		if (printf("%s=%s\n", env_content->key, env_content->value) < 0)
// 			return (1);
// 		env_list_tmp = env_list_tmp->next;
// 	}
// 	return (0);
// }
