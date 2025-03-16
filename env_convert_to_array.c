/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert_to_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:56:15 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 14:56:44 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char **convert_env_list_to_array(t_list *env)
{
    int env_count = 0;
    t_list *current = env;
    char **env_array;
    t_env *env_node;

    // 環境変数の数を数える
    while (current)
    {
        env_count++;
        current = current->next;
    }

    // 配列のメモリ確保（NULL終端用に+1）
    env_array = malloc(sizeof(char *) * (env_count + 1));
    if (!env_array)
        return NULL;

    // 環境変数をkey=value形式でコピー
    current = env;
    env_count = 0;
    while (current)
    {
        env_node = (t_env *)current->content;
        // key=valueの形式で文字列を作成
        env_array[env_count] = ft_strjoin3(env_node->key, "=", env_node->value);
        if (!env_array[env_count])
        {
            // エラー時のクリーンアップ
            while (--env_count >= 0)
                free(env_array[env_count]);
            free(env_array);
            return NULL;
        }
        env_count++;
        current = current->next;
    }
    env_array[env_count] = NULL;  // NULL終端

    return env_array;
}