/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:48:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/18 12:19:49 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	print_env(t_list *env_list);

int	builtin_env(char **cmd, t_list *env)
{
	(void)cmd;
	return (print_env(env));
}

static int	print_env(t_list *env_list)
{
	t_list	*env_list_tmp;
	t_env	*env_content;

	env_list_tmp = env_list;
	while (env_list_tmp)
	{
		env_content = (t_env *)env_list_tmp->content;
		env_list_tmp = env_list_tmp->next;
		if (env_content->is_shell_var)
			continue ;
		if (printf("%s=%s\n", env_content->key, env_content->value) < 0)
			return (1);
	}
	return (0);
}
