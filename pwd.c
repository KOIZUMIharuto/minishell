/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:41 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 01:51:55 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_pwd(char **args) {
    (void)args;
    char *buffer;
    size_t size = 1024;

    buffer = (char *)malloc(size);
    if (!buffer) {
        perror("malloc");
        return (1);
    }
    if (getcwd(buffer, size) == NULL) {
        perror("getcwd");
        free(buffer);
        return (1);
    }
    printf("%s\n", buffer);
    free(buffer);
    return (0);
}

