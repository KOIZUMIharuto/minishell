/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_to_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:44:47 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/21 15:50:01 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_cmd	*tokens_to_cmd(t_list *tokens, t_parser data);
static bool		get_cmd(char ***cmd, t_list **tokens, t_parser data);
static void		copy_list_to_cmd(char **cmd, t_list *tokens);

t_list	*splited_tokens_to_cmd_list(t_list **splited_tokens, t_parser data)
{
	t_list	*cmd_list;
	t_list	*cmd_node;
	t_cmd	*cmd;
	int		i;

	cmd_list = NULL;
	i = -1;
	while (splited_tokens[++i])
	{
		cmd = tokens_to_cmd(splited_tokens[i], data);
		if (!cmd)
		{
			ft_lstclear(&cmd_list, free);
			return (NULL);
		}
		cmd_node = ft_lstnew(cmd);
		if (!cmd_node)
		{
			free(cmd);
			ft_lstclear(&cmd_list, free);
			return ((t_list *)perror_ptr("malloc", errno));
		}
		ft_lstadd_back(&cmd_list, cmd_node);
	}
	free(splited_tokens);
	return (cmd_list);
}

static t_cmd	*tokens_to_cmd(t_list *tokens, t_parser data)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (!get_rdrcts(&(cmd->rdrcts), &tokens, data)
		|| !get_cmd(&(cmd->cmd), &tokens, data))
	{
		free_cmd((void *)cmd);
		return (NULL);
	}
	cmd->infile_fd = -1;
	cmd->outfile_fd = -1;
	cmd->backup_stdin = -1;
	cmd->backup_stdout = -1;
	return (cmd);
}

static bool	get_cmd(char ***cmd, t_list **tokens, t_parser data)
{
	t_list	*expanded_list;
	t_list	*tmp;

	expanded_list = NULL;
	while (*tokens)
	{
		data.is_failed = true;
		tmp = NULL;
		if (!expand_env_quote(&tmp, (char *)(*tokens)->content, &data))
		{
			ft_lstclear(&expanded_list, free);
			return (false);
		}
		ft_lstadd_back(&expanded_list, tmp);
		tmp = *tokens;
		*tokens = (*tokens)->next;
	}
	// if (ft_lstsize(expanded_list) == 0)
	// {
	// 	ft_lstclear(&expanded_list, free);
	// 	return (false);
	// }
	*cmd = (char **)ft_calloc(ft_lstsize(expanded_list) + 1, sizeof(char *));
	if (!*cmd)
	{
		ft_lstclear(&expanded_list, free);
		return (perror_bool("malloc", errno));
	}
	copy_list_to_cmd(*cmd, expanded_list);
	return (true);
}

static void	copy_list_to_cmd(char **cmd, t_list *expanded_list)
{
	t_list	*tmp;
	int		i;

	i = -1;
	while (expanded_list)
	{
		cmd[++i] = (char *)expanded_list->content;
		tmp = expanded_list;
		expanded_list = expanded_list->next;
		free(tmp);
	}
}
