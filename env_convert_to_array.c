/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert_to_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:56:15 by shiori            #+#    #+#             */
/*   Updated: 2025/03/21 13:52:25 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	count_env(t_list *env);
static char	*ft_strjoin3(char *s1, char *s2, char *s3);

char	**convert_env_list_to_array(t_list *env)
{
	int		env_count;
	char	**env_array;
	t_env	*env_node;

	env_count = count_env(env);
	env_array = (char **)ft_calloc(env_count + 1, sizeof(char *));
	if (!env_array)
		return ((char **)perror_ptr("malloc", errno));
	env_array[env_count] = NULL;
	env_count = 0;
	while (env)
	{
		env_node = (t_env *)env->content;
		env = env->next;
		if (env_node->is_hidden)
			continue ;
		env_array[env_count] = ft_strjoin3(env_node->key, "=", env_node->value);
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

static char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return ((char *)perror_ptr("malloc", errno));
	result = ft_strjoin(tmp, s3);
	free(tmp);
	if (!result)
		return ((char *)perror_ptr("malloc", errno));
	return (result);
}
