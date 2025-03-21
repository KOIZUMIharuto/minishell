/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 03:26:07 by shiori            #+#    #+#             */
/*   Updated: 2025/03/22 02:28:11 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void free_double_pointor(char **array) {
    int i;

    i = 0;
    while (array[i]) {
        free(array[i]);
        i++;
    }
    free(array);
}

void	free_data(t_data data)
{
	ft_lstclear(&data.env, env_free);
	free_cmds(data.cmds);
	free(data.pids);
}
