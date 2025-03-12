/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:54:17 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 02:44:19 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int builtin_exit(char **args) {
    int exit_status = g_last_exit_status;

    write(STDOUT_FILENO, "exit\n", 5);

    if (args[1] != NULL) {
        if (!is_numeric(args[1])) {
            write(STDERR_FILENO, "exit: ", 6);
            write(STDERR_FILENO, args[1], ft_strlen(args[1]));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            exit_status = 255;
            return(-1);
        }
        if (args[2] != NULL) {
            write(STDERR_FILENO, "exit: too many arguments\n", 26);
            g_last_exit_status = 1;
            return (1);
        }
        exit_status = ft_atoi(args[1]) % 256;
    }
    g_last_exit_status = exit_status;
    return (0);
}
