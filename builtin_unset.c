/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:11:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/17 14:33:38 by hkoizumi         ###   ########.fr       */
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
		if (!is_valid_key(cmd[i]) || !env_delete(&env, cmd[i]))
				status = 1;
		i++;
	}
	return (status);
}
