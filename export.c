/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:44:22 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 02:11:17 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *create_env_entry(char *var, char *value) {
    char *entry;
    size_t len = ft_strlen(var) + (value ? ft_strlen(value) + 1 : 0);

    entry = malloc(len + 1);
    if (!entry)
        return (NULL);
    ft_strlcpy(entry, var, len + 1);
    if (value) {
        ft_strlcat(entry, "=", len + 1);
        ft_strlcat(entry, value, len + 1);
    }
    return (entry);
}

int update_env(char *new_entry, size_t var_len) {
    char **env = environ;

    while (*env) {
        if (ft_strncmp(*env, new_entry, var_len) == 0 && ((*env)[var_len] == '=' || (*env)[var_len] == '\0')) {
            *env = new_entry;
            return (1);
        }
        env++;
    }
    return (0);
}

void add_env(char *new_entry) {
    size_t count = 0;
    while (environ[count])
        count++;

    char **new_environ = malloc((count + 2) * sizeof(char *));
    if (!new_environ) {
        perror("malloc");
        free(new_entry);
        return;
    }
    size_t i = 0;
    while (i < count) {
        new_environ[i] = environ[i];
        i++;
    }
    new_environ[count] = new_entry;
    new_environ[count + 1] = NULL;
    environ = new_environ;
}

int add_or_update_env(char *var, char *value) {
    char *new_entry;
    size_t var_len;
    char **env;
    char *command;

    if (!is_valid_identifier(var)) {
        command="export";
        handle_invalid_identifier(command,var);
        return (1);
    }
    var_len = ft_strlen(var);
    env = environ;
    while (*env) {
        if (ft_strncmp(*env, var, var_len) == 0 && ((*env)[var_len] == '=' || (*env)[var_len] == '\0')) {
            if (value == NULL)
                return (0);
            break;
        }
        env++;
    }
    new_entry = create_env_entry(var, value);
    if (!new_entry) {
        perror("malloc");
        return (1);
    }
    if (!update_env(new_entry, var_len))
        add_env(new_entry);
    return (0);
}

void print_env_quoted(char *env) {
    char *equal_pos = ft_strchr(env, '=');
    if (equal_pos) {
        printf("declare -x %.*s=\"%s\"\n", (int)(equal_pos - env), env, equal_pos + 1);
    } else {
        printf("declare -x %s\n", env);
    }
}

int builtin_export(char **args) {
    int i = 1;
    int status;

    status=0;
    if (!args[1]) {
        char **env = environ;
        while (*env) {
            print_env_quoted(*env);
            env++;
        }
        return (status);
    }
    while (args[i]) {
        char *equal_sign = ft_strchr(args[i], '=');
        if (equal_sign) {
            *equal_sign = '\0';
            if (add_or_update_env(args[i], equal_sign + 1) != 0) {
                status = 1;
            }
        } else {
            if (add_or_update_env(args[i], NULL) != 0) {
                status = 1;
            }
        }
        i++;
    }
    return (status);
}
