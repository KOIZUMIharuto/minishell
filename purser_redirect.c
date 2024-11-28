/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:15:15 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/28 12:28:47 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static int	set_redirection_type(char *key_pos, char key, t_redirection *redirection);

t_redirection	**check_redirection(char *line, char *key, int redirect_cnt)
{
	t_redirection	**redirections;
	t_redirection	*redirection;
	t_quote			quote;
	char			*key_pos;
	char			*key_pos_tmp;
	char			*file_name_tmp;
	int				i;

	if (!line)
		return (NULL);
	key_pos = line;
	while (*key_pos && !is_del(*key_pos, key, NULL))
			key_pos++;
	if (!*key_pos)
	{
		redirections = (t_redirection **)ft_calloc(redirect_cnt + 1, sizeof(t_redirection *));
		if (!redirections)
			return (NULL);
	}
	else
	{
		redirection = (t_redirection *)ft_calloc(1, sizeof(t_redirection));
		if (!redirection)
			return (NULL);
		key_pos_tmp = key_pos++;
		key_pos += set_redirection_type(key_pos, key[0], redirection);
		while (*key_pos && is_del(*key_pos, " \t\n", NULL))
			key_pos++;
		quote = NONE_QUOTE;
		i = 0;
		while (key_pos[i] && !is_del(key_pos[i], " \t\n", &quote))
			i++;
		if (key_pos[0])
		{
			file_name_tmp = ft_substr(key_pos, 0, i);
			if (!file_name_tmp)
			{
				free(redirection);
				return (NULL);
			}
			if (redirection->type == HEREDOCUMENT)
			{
				redirection->file = (char **)ft_calloc(2, sizeof(char *));
				if (!redirection->file)
				{
					free(file_name_tmp);
					free(redirection);
					return (NULL);
				}
				redirection->file[0] = file_name_tmp;
			}
			else
			{
				redirection->file = split_arg(file_name_tmp);
				free(file_name_tmp);
				if (!redirection->file)
				{
					free(redirection);
					return (NULL);
				}
			}
		}
		else
			redirection->file = NULL;
		redirection->fd = -1;
		(void)ft_strlcpy(key_pos_tmp, key_pos + i, ft_strlen(key_pos_tmp) + 1);
		redirections = check_redirection(line, key, redirect_cnt + 1);
		if (!redirections)
		{
			free_redirection(redirection);
			return (NULL);
		}
		redirections[redirect_cnt] = redirection;
	}
	return (redirections);
}

static int	set_redirection_type(char *key_pos, char key, t_redirection *redirection)
{
	if (key == '<')
	{
		if (key_pos[0] == '<')
		{
			redirection->type = HEREDOCUMENT;
			return (1);
		}
		else
			redirection->type = INPUT_REDIRECTION;
	}
	else
	{
		if (key_pos[0] == '>')
		{
			redirection->type = APPEND_REDIRECTION;
			return (1);
		}
		else
			redirection->type = OVERWRITE_REDIRECTION;
	}
	return (0);
}