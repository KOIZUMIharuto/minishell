/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:28 by shiori            #+#    #+#             */
/*   Updated: 2025/03/15 03:00:19 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int builtin_echo(char **cmd, t_list *env) {
    int i;
    int newline;

	(void)env;
    i = 1;
    newline = 1;
    if (cmd[1] && ft_strcmp(cmd[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }
    while (cmd[i]) {
        write(STDOUT_FILENO, cmd[i], strlen(cmd[i]));
        if (cmd[i + 1]) {
            write(STDOUT_FILENO, " ", 1);
        }
        i++;
    }
    if (newline) {
        write(STDOUT_FILENO, "\n", 1);
    }
    return (0);
}
