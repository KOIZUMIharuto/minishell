/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:51:27 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 02:52:58 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
