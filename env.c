/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:48:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 01:55:46 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(void) {
    char **env;
    env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

int builtin_env(char **args) {
    (void)args;
    print_env();
    return (0);
}