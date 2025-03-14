/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:21:02 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/14 13:07:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static t_list	*purse_to_list(char *line, t_data *data);
static bool		handle_quotes_and_env(t_list **args, char *arg, t_data *data);

char	**split_arg(char *line, t_data *data)
{
	t_list	*args;
	t_list	*tmp;
	int		i;
	char	**cmd;

	if (!line)
		return (NULL);
	args = purse_to_list(line, data);
	if (args)
		cmd = (char **)malloc(sizeof(char *) * (ft_lstsize(args) + 1));
	if (!args || !cmd)
	{
		ft_lstclear(&args, free);
		return (NULL);
	}
	i = 0;
	while (args)
	{
		tmp = args;
		cmd[i++] = args->content;
		args = args->next;
		free(tmp);
	}
	cmd[i] = NULL;
	return (cmd);
}

static t_list	*purse_to_list(char *line, t_data *data)
{
	t_list	*args;
	t_quote	quote;
	int		i;

	if (!line)
		return (NULL);
	args = NULL;
	while (*line)
	{
		while (*line && is_del(*line, " \t\n", NULL))
			line++;
		if (!*line)
			break ;
		i = 0;
		quote = NONE_Q;
		while (line[i] && !is_del(line[i], " \t\n", &quote))
			i++;
		if (!handle_quotes_and_env(&args, ft_substr(line, 0, i), data))
		{
			ft_lstclear(&args, free);
			return (NULL);
		}
		line += i;
	}
	return (args);
}

static bool	handle_quotes_and_env(t_list **args, char *arg, t_data *data)
{
	char	*arg_head;
	t_list	*tmp;
	char	*arg_tmp;

	arg_head = arg;
	tmp = NULL;
	while (arg && (*arg || (data->tmp && *(data->tmp))))
	{
		arg_tmp = recursive_expand(&arg, data, NONE_Q, 0);
		if (!arg_tmp)
			break ;
		tmp = ft_lstnew(arg_tmp);
		if (!tmp)
		{
			free (arg_tmp);
			break ;
		}
		ft_lstadd_back(args, tmp);
	}
	free(arg_head);
	if (!arg || !arg_tmp || !tmp)
		return (false);
	return (true);
}
