/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:59:46 by shiori            #+#    #+#             */
/*   Updated: 2025/03/14 00:09:03 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
  
int interpret_line(char *line) {
    char **tokens = ft_split(line, '|'); // パイプで分割
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        return (0);
    }
    // 内部コマンドの実行または外部コマンドの実行
    int status;
    if (tokens[0] != NULL) {
        // 内部コマンドかを判断、内部コマンドの場合はインデックスを渡す
        status=pipe_command(tokens);
    }
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);

    if (WIFEXITED(status)) {
        g_last_exit_status = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        g_last_exit_status = 128 + WTERMSIG(status);
    }

    return (status);
}

int g_last_exit_status = 0;

int main() {
    char *line;

    setup_signal_handlers();

    while (1) {
        line = readline("$ ");
        if (!line) {
            write(1, "exit\n", 5);
            break;
        }
        if (ft_strlen(line) > 0) {
            add_history(line);
            int status=interpret_line(line);
            if (status == -42) {
                free(line);
                exit(g_last_exit_status);
            }
        }
        free(line);
    }
    return (0);
}

