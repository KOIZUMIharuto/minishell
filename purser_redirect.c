/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_rdrct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:15:15 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/29 14:48:11 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_rdrct	*set_rdrct(char **key_pos, char *key, t_purser *data);
static int		set_rdrct_type(char *key_pos, char key, t_rdrct *rdrct);
static bool		set_file(char *key_pos, t_rdrct *rdrct, int len, t_purser *data);

t_rdrct	**check_rdrct(char *line, char *key, int rdrct_cnt, t_purser *data)
{
	t_rdrct	**rdrcts;
	t_rdrct	*rdrct;
	char	*key_pos;

	if (!line)
		return (NULL);
	key_pos = line;
	while (*key_pos && !is_del(*key_pos, key, NULL))
		key_pos++;
	if (!*key_pos)
		rdrcts = (t_rdrct **)ft_calloc(rdrct_cnt + 1, sizeof(t_rdrct *));
	else
	{
		rdrct = set_rdrct(&key_pos, key, data);
		if (!rdrct)
			return (NULL);
		rdrcts = check_rdrct(line, key, rdrct_cnt + 1, data);
		if (!rdrcts)
			free_rdrct(rdrct);
		else
			rdrcts[rdrct_cnt] = rdrct;
	}
	return (rdrcts);
}

static t_rdrct	*set_rdrct(char **key_pos, char *key, t_purser *data)
{
	t_rdrct	*rdrct;
	t_quote	quote;
	char	*key_pos_tmp;
	int		i;

	rdrct = (t_rdrct *)ft_calloc(1, sizeof(t_rdrct));
	if (!rdrct)
		return (NULL);
	key_pos_tmp = *key_pos;
	*key_pos += set_rdrct_type(*key_pos, key[0], rdrct);
	while (**key_pos && is_del(**key_pos, " \t\n", NULL))
		(*key_pos)++;
	quote = NONE_Q;
	i = 0;
	while ((*key_pos)[i] && !is_del((*key_pos)[i], " \t\n><", &quote))
		i++;
	if (!set_file(*key_pos, rdrct, i, data))
	{
		free_rdrct (rdrct);
		return (NULL);
	}
	rdrct->fd = -1;
	(void)ft_strlcpy(key_pos_tmp, *key_pos + i, ft_strlen(key_pos_tmp) + 1);
	return (rdrct);
}

static int	set_rdrct_type(char *key_pos, char key, t_rdrct *rdrct)
{
	if (key == '<')
	{
		if (key_pos[1] == '<')
		{
			rdrct->type = HEREDOCUMENT;
			return (2);
		}
		else
			rdrct->type = INPUT_RDRCT;
	}
	else
	{
		if (key_pos[1] == '>')
		{
			rdrct->type = APPEND_RDRCT;
			return (2);
		}
		else
			rdrct->type = OVERWRITE_RDRCT;
	}
	return (1);
}

static bool	set_file(char *key_pos, t_rdrct *rdrct, int len, t_purser *data)
{
	char	*file_name_tmp;

	if (key_pos[0])
	{
		if (rdrct->type == HEREDOCUMENT)
		{
			rdrct->file = (char **)ft_calloc(2, sizeof(char *));
			if (rdrct->file)
				rdrct->file[0] = ft_substr(key_pos, 0, len);
		}
		else
		{
			file_name_tmp = ft_substr(key_pos, 0, len);
			if (!file_name_tmp)
				return (false);
			rdrct->file = split_arg(file_name_tmp, data);
			free(file_name_tmp);
		}
		if (!rdrct->file || !rdrct->file[0])
			return (false);
	}
	else
		rdrct->file = NULL;
	return (true);
}
