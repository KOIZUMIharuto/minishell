/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:54:16 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 17:30:34 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void		init_parser(t_parser *data, int exit_status, t_list *env);
static t_cmd	**list_to_cmds(t_list *cmd_list);
static void		free_splited_tokens(t_list ***splited_tokens);

t_cmd	**parser(char *line, int exit_status, t_list *env)
{
	t_parser	data;
	t_list		*tokens;
	t_list		**splited_tokens;
	t_list		*cmd_list;
	t_cmd		**cmds;

	init_parser(&data, exit_status, env);
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens) || !split_tokens(&splited_tokens, tokens))
	{
		ft_lstclear(&tokens, free);
		return (NULL);
	}
	cmd_list = splited_tokens_to_cmd_list(splited_tokens, data);
	free_splited_tokens(&splited_tokens);
	cmds = list_to_cmds(cmd_list);
	if (!cmds)
	{
		ft_lstclear(&cmd_list, free_cmd);
		return (NULL);
	}
	return (cmds);
}

static void	init_parser(t_parser *data, int exit_status, t_list *env)
{
	int	i;
	int	tmp;
	int	div;

	exit_status = (exit_status % 256 + 256) % 256;
	i = 0;
	div = 100;
	while (div)
	{
		tmp = exit_status / div;
		if (tmp == 0 && i == 0 && div != 1)
		{
			div /= 10;
			continue ;
		}
		data->exit_status[i++] = tmp + '0';
		exit_status %= div;
		div /= 10;
	}
	while (i <= 3)
		data->exit_status[i++] = '\0';
	data->env = env;
	data->is_empty_env_exist = false;
	data->is_failed = false;
	data->tmp = NULL;
}


static t_cmd	**list_to_cmds(t_list *cmd_list)
{
	t_cmd	**cmds;
	t_list	*tmp;
	int		cmd_count;

	if (!cmd_list)
		return (NULL);
	cmd_count = ft_lstsize(cmd_list);
	cmds = (t_cmd **)ft_calloc(cmd_count + 1, sizeof(t_cmd *));
	if (!cmds)
	{
		ft_lstclear(&cmd_list, free_cmd);
		return ((t_cmd **)perror_ptr("malloc", errno));
	}
	cmd_count = -1;
	while (cmd_list)
	{
		cmds[++cmd_count] = (t_cmd *)cmd_list->content;
		tmp = cmd_list->next;
		free(cmd_list);
		cmd_list = tmp;
	}
	return (cmds);
}

static void	free_splited_tokens(t_list ***splited_tokens)
{
	int	i;

	i = 0;
	while ((*splited_tokens)[i])
		ft_lstclear(&(*splited_tokens)[i++], free);
	free(*splited_tokens);
	*splited_tokens = NULL;
}
