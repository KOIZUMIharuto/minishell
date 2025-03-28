/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:11:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 19:34:45 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_valid	builtin_unset(char **cmd, t_list *env)
{
	int		i;
	t_valid	is_valid;
	t_valid	valid_status;

	i = 0;
	valid_status = VALID;
	while (cmd[++i])
	{
		is_valid = is_valid_key(cmd[i]);
		if (is_valid == ERROR)
			return (ERROR);
		else if (is_valid == INVALID)
		{
			valid_status = INVALID;
			continue ;
		}
		env_delete(&env, cmd[i]);
	}
	return (valid_status);
}
