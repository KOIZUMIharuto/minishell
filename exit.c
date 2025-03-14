/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:54:17 by shiori            #+#    #+#             */
/*   Updated: 2025/03/15 03:01:05 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

extern int g_last_exit_status;

int is_numeric(const char *str) {
    int i = 0;

    if (!str || !str[0])
        return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i]) {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int builtin_exit(char **cmd, t_list *env) {
    int exit_status = g_last_exit_status;

	(void)env;
    write(STDOUT_FILENO, "exit\n", 5);

    if (cmd[1] != NULL) {
        if (!is_numeric(cmd[1])) {
            write(STDERR_FILENO, "exit: ", 6);
            write(STDERR_FILENO, cmd[1], ft_strlen(cmd[1]));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            exit_status = 255;
            return(-1);
        }
        if (cmd[2] != NULL) {
            write(STDERR_FILENO, "exit: too many arguments\n", 26);
            g_last_exit_status = 1;
            return (1);
        }
        exit_status = ft_atoi(cmd[1]) % 256;
    }
    g_last_exit_status = exit_status;
    return (0);
}
