/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert_to_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:56:15 by shiori            #+#    #+#             */
/*   Updated: 2025/03/25 13:14:31 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	count_env(t_list *env);
static char	*env_join(char *key, char *value);

char	**convert_env_list_to_array(t_list *env)
{
	int		env_count;
	char	**env_array;
	t_env	*env_node;

	env_count = count_env(env);
	env_array = (char **)ft_calloc(env_count + 1, sizeof(char *));
	if (!env_array)
		return ((char **)perror_ptr("malloc"));
	env_array[env_count] = NULL;
	env_count = 0;
	while (env)
	{
		env_node = (t_env *)env->content;
		env = env->next;
		if (env_node->is_hidden)
			continue ;
		env_array[env_count] = env_join(env_node->key, env_node->value);
		if (!env_array[env_count])
		{
			free_double_pointor(env_array);
			return (NULL);
		}
		env_count++;
	}
	return (env_array);
}

static int	count_env(t_list *env)
{
	int		count;
	t_env	*env_node;

	count = 0;
	while (env)
	{
		env_node = (t_env *)env->content;
		env = env->next;
		if (env_node->is_hidden)
			continue ;
		count++;
	}
	return (count);
}

static char	*env_join(char *key, char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return ((char *)perror_ptr("malloc"));
	result = ft_strjoin(tmp, value);
	free(tmp);
	if (!result)
		return ((char *)perror_ptr("malloc"));
	return (result);
}
