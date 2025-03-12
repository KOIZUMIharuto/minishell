/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:37:35 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 02:11:57 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_error(char *command, char *arg, int error_code) {
    write(STDERR_FILENO, command, strlen(command));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, arg, strlen(arg));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, strerror(error_code), strlen(strerror(error_code)));
    write(STDERR_FILENO, "\n", 1);
}

int builtin_cd(char **args) {
    struct stat path_stat;
    char *home_dir;

    if (args[1] == NULL) {
        home_dir = getenv("HOME");
        if (!home_dir) {
            write(STDERR_FILENO, "cd: HOME not set\n", 17);
            return (1);
        }
        args[1] = home_dir;
    }
    if (stat(args[1], &path_stat) != 0) {
        print_error("cd", args[1], ENOENT);
        return (1);
    }
    if (!S_ISDIR(path_stat.st_mode)) {
        print_error("cd", args[1], ENOTDIR);
        return (1);
    }
    if (chdir(args[1]) != 0) {
        print_error("cd", args[1], EACCES);
        return (1);
    }
    return (0);
}
