/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:28 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 01:39:45 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_echo(char **args) {
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (args[1] && ft_strcmp(args[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }
    while (args[i]) {
        write(STDOUT_FILENO, args[i], strlen(args[i]));
        if (args[i + 1]) {
            write(STDOUT_FILENO, " ", 1);
        }
        i++;
    }
    if (newline) {
        write(STDOUT_FILENO, "\n", 1);
    }
    return (0);
}
