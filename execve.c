/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 03:20:59 by shiori            #+#    #+#             */
/*   Updated: 2025/03/17 22:32:03 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR));
}

char *concat_path_cmd(char *dir, char *cmd) {
    int total_len;
    char *cmd_path;
    
    total_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
    cmd_path = malloc(total_len);
    if (!cmd_path)
        return NULL;
    ft_strlcpy(cmd_path, dir, total_len);
    ft_strlcat(cmd_path, "/", total_len);
    ft_strlcat(cmd_path, cmd, total_len);
    return cmd_path;
}

char *find_cmd_path(char *cmd, t_list *env) {
	t_env	*path_env;
    char **path_dirs;
    char *cmd_path;
    int i;
    
	path_env = env_get(env, "PATH", false);
	if (!path_env || !path_env->value)
		return (cmd);
    path_dirs = ft_split(path_env->value, ':');
    if (!path_dirs) 
        return NULL;
    i=0;
    while (path_dirs[i]) {
        cmd_path = concat_path_cmd(path_dirs[i], cmd);
        if (is_executable(cmd_path)) {
            free_double_pointor(path_dirs);
            return cmd_path;
        }
        free(cmd_path);
        i++;
    }
    free_double_pointor(path_dirs);
    return NULL;
}

void execute_cmd(char **cmd, t_list *env)
{
    char *cmd_path;
    char **env_array;

    cmd_path = (ft_strchr(cmd[0], '/') != NULL) ? cmd[0] : find_cmd_path(cmd[0], env);
    if (!cmd_path)
    {
        write(STDERR_FILENO, cmd[0], ft_strlen(cmd[0]));
        write(STDERR_FILENO, ": command not found\n", 20);  
        exit(EXIT_FAILURE);
    }

    env_array = convert_env_list_to_array(env);
    if (!env_array)
    {
        if (cmd_path != cmd[0])
            free(cmd_path);
        exit(EXIT_FAILURE);
    }

    if (execve(cmd_path, cmd, env_array) == -1)
    {
        (void)error_msg(cmd_path, strerror(errno));
        if (cmd_path != cmd[0])
            free(cmd_path);	// cmd_pathをfreeするならenv_arrayもfreeして良いのでは
		free_double_pointor(env_array);
        // env_arrayのクリーンアップは不要（execve成功時はプロセスが置き換わり、
        // 失敗時はexit()で終了するため）<- ?
        exit(EXIT_FAILURE);
    }
}








