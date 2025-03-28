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
static void		set_exit_status(t_parser *data, int exit_status);
static t_cmd	**list_to_cmds(t_list *cmd_list);
static void		free_splited_tokens(t_list ***splited_tokens);

t_valid	parser(t_cmd ***cmds, char *line, int exit_status, t_list *env)
{
	t_parser	data;
	t_valid		valid_status;
	t_list		*tokens;
	t_list		**splited_tokens;
	t_list		*cmd_list;

	init_parser(&data, exit_status, env);
	tokens = NULL;
	if (!tokenize(&tokens, line))
		return (CRITICAL_ERROR);
	valid_status = check_syntax(tokens);
	if (valid_status == VALID && !split_tokens(&splited_tokens, tokens))
		valid_status = CRITICAL_ERROR;
	if (valid_status != VALID)
		return (valid_status);
	cmd_list = to_cmd_list(splited_tokens, data);
	free_splited_tokens(&splited_tokens);
	*cmds = list_to_cmds(cmd_list);
	if (!*cmds)
	{
		ft_lstclear(&cmd_list, free_cmd);
		return (CRITICAL_ERROR);
	}
	return (VALID);
}

static void	init_parser(t_parser *data, int exit_status, t_list *env)
{
	t_env	*ifs_env;

	set_exit_status(data, exit_status);
	data->env = env;
	ifs_env = env_get(env, "IFS", false);
	if (ifs_env)
		data->del = ifs_env->value;
	else
		data->del = " \t\n";
	data->is_env_empty = false;
	data->is_failed = false;
	data->tmp = NULL;
}

static void	set_exit_status(t_parser *data, int exit_status)
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
		return ((t_cmd **)perror_ptr("malloc"));
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
