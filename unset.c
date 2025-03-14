/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:11:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/15 03:07:45 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_unset(char **cmd, t_list *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd[i])
	{
		if (!is_valid_key(cmd[i]))
		{
			handle_invalid_key(cmd[0], cmd[i]);
			status = 1;
		}
		else
			env_delete(&env, cmd[i]);
		i++;
	}
	return (status);
}
