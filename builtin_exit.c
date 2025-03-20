/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:54:17 by shiori            #+#    #+#             */
/*   Updated: 2025/03/21 01:27:29 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// extern int g_last_exit_status;

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

	(void)env;

    if (cmd[1] && cmd[2]) {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        g_last_exit_status = 1;
        return (-1);
    }
     if (cmd[1] && !is_numeric(cmd[1])) {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(cmd[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        g_last_exit_status = 2;  // Linuxでは終了コード2
        exit(g_last_exit_status);
    }
    g_last_exit_status = ft_atoi(cmd[1]) & 0xFF;  // ビットマスクで確実に0-255に制限;
    write(STDOUT_FILENO, "exit\n", 5);
    return (0);
}
