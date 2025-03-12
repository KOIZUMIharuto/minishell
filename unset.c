/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:11:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 01:54:39 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

void remove_env(char *var) {
    size_t var_len = ft_strlen(var);
    char **env = environ;
    char **target = NULL;

    while (*env) {
        if (ft_strncmp(*env, var, var_len) == 0 && (*env)[var_len] == '=') {
            target = env;
            break;
        }
        env++;
    }
    if (!target)
        return;

    while (*target) {
        *target = *(target + 1);
        target++;
    }
}

int builtin_unset(char **args) {
    int i;
    char *command = "unset";
    int status;

    i = 1;
    status = 0;
    while (args[i]) {
        if (!is_valid_identifier(args[i])) {
            handle_invalid_identifier(command, args[i]);
            status = 1;
        } else {
            remove_env(args[i]);
        }
        i++;
    }
    return (status);
}
