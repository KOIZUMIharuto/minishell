/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 03:20:59 by shiori            #+#    #+#             */
/*   Updated: 2025/03/13 22:30:32 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR));
}

char *concat_path_command(char *dir, char *command) {
    int total_len;
    char *cmd_path;
    
    total_len = ft_strlen(dir) + ft_strlen(command) + 2;
    cmd_path = malloc(total_len);
    if (!cmd_path)
        return NULL;
    ft_strlcpy(cmd_path, dir, total_len);
    ft_strlcat(cmd_path, "/", total_len);
    ft_strlcat(cmd_path, command, total_len);
    return cmd_path;
}

char *find_command_path(char *command) {
    char *path_env;
    char **path_dirs;
    char *command_path;
    int i;
    
    path_env = getenv("PATH");
    if (!path_env) 
        return NULL;
    path_dirs = ft_split(path_env, ':');
    if (!path_dirs) 
        return NULL;
    i=0;
    while (path_dirs[i]) {
        command_path = concat_path_command(path_dirs[i], command);
        if (is_executable(command_path)) {
            free_double_pointor(path_dirs);
            return command_path;
        }
        free(command_path);
        i++;
    }
    free_double_pointor(path_dirs);
    return NULL;
}

void execute_command(char **command)
{
    char *command_path;

    command_path = (ft_strchr(command[0], '/') != NULL) ? command[0] : find_command_path(command[0]);
    if (!command_path)
    {
        write(STDERR_FILENO, command[0], ft_strlen(command[0]));
        write(STDERR_FILENO, ": command not found\n", 20);  
        exit(EXIT_FAILURE);
    }
    if (execve(command_path, command, environ) == -1)
    {
        perror("execve");
        if (command_path != command[0])
            free(command_path);
        exit(EXIT_FAILURE);
    }
}





