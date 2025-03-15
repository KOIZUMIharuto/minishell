/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:37:26 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 22:50:06 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_builtins(t_builtin *builtins)
{
	builtins[0] = (t_builtin){"echo", builtin_echo};
	builtins[1] = (t_builtin){"cd", builtin_cd};
	builtins[2] = (t_builtin){"pwd", builtin_pwd};
	builtins[3] = (t_builtin){"exit", builtin_exit};
	builtins[4] = (t_builtin){"export", builtin_export};
	builtins[5] = (t_builtin){"unset", builtin_unset};
	builtins[6] = (t_builtin){"env", builtin_env};
}

int	get_builtin_index(t_builtin *builtins, char *cmd)
{
	int	i;

	i = 0;
	while (i < BUILTIN_NUM)
	{
		if (ft_strcmp(builtins[i].name, cmd) == 0)
			return (i);
		i++;
	}
	return (-1);
}
