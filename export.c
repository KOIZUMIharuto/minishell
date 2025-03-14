/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:44:22 by shiori            #+#    #+#             */
/*   Updated: 2025/03/15 03:44:58 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	export_env(t_list *env_list);

int	builtin_export(char **cmd, t_list *env)
{
	int		i;
	int		status;
	char	*key;

	if (!cmd[1])
		return (export_env(env));
	i = 1;
	status = 0;
	while (cmd[i])
	{
		key = env_update(&env, cmd[i]);
		if (!is_valid_key(key))
		{
			handle_invalid_key(cmd[0], key);
			free(key);
			status = 1;
		}
		i++;
	}
	return (status);
}

static int	export_env(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (ft_strcmp(env->key, "_") != 0)
		{
			env_list = env_list->next;
			continue ;
		}
		if (printf("export %s=", env->key) < 0)
			return (1);
		if (env->value && printf("\"%s\"", env->value) < 0)
			return (1);
		if (printf("\n") < 0)
			return (1);
		env_list = env_list->next;
	}
	return (0);
}

// char *create_env_entry(char *var, char *value) {
//     char *entry;
//     size_t len = ft_strlen(var) + (value ? ft_strlen(value) + 1 : 0);

//     entry = malloc(len + 1);
//     if (!entry)
//         return (NULL);
//     ft_strlcpy(entry, var, len + 1);
//     if (value) {
//         ft_strlcat(entry, "=", len + 1);
//         ft_strlcat(entry, value, len + 1);
//     }
//     return (entry);
// }

// int update_env(char *new_entry, size_t var_len) {
//     char **env = environ;

//     while (*env) {
//         if (ft_strncmp(*env, new_entry, var_len) == 0 && ((*env)[var_len] == '=' || (*env)[var_len] == '\0')) {
//             *env = new_entry;
//             return (1);
//         }
//         env++;
//     }
//     return (0);
// }

// void add_env(char *new_entry) {
//     size_t count = 0;
//     while (environ[count])
//         count++;

//     char **new_environ = malloc((count + 2) * sizeof(char *));
//     if (!new_environ) {
//         perror("malloc");
//         free(new_entry);
//         return;
//     }
//     size_t i = 0;
//     while (i < count) {
//         new_environ[i] = environ[i];
//         i++;
//     }
//     new_environ[count] = new_entry;
//     new_environ[count + 1] = NULL;
//     environ = new_environ;
// }

// int add_or_update_env(char *var, char *value) {
//     char *new_entry;
//     size_t var_len;
//     char **env;
//     char *command;

//     if (!is_valid_key(var)) {
//         command="export";
//         handle_invalid_key(command,var);
//         return (1);
//     }
//     var_len = ft_strlen(var);
//     env = environ;
//     while (*env) {
//         if (ft_strncmp(*env, var, var_len) == 0 && ((*env)[var_len] == '=' || (*env)[var_len] == '\0')) {
//             if (value == NULL)
//                 return (0);
//             break;
//         }
//         env++;
//     }
//     new_entry = create_env_entry(var, value);
//     if (!new_entry) {
//         perror("malloc");
//         return (1);
//     }
//     if (!update_env(new_entry, var_len))
//         add_env(new_entry);
//     return (0);
// }

// void print_env_quoted(char *env) {
//     char *equal_pos = ft_strchr(env, '=');
//     if (equal_pos) {
//         printf("declare -x %.*s=\"%s\"\n", (int)(equal_pos - env), env, equal_pos + 1);
//     } else {
//         printf("declare -x %s\n", env);
//     }
// }
