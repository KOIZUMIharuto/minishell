/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:08:45 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/31 23:51:53 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
		free_cmd((void *)cmds[i++]);
	free(cmds);
}

void	free_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	if (cmd->cmd)
		free_double_pointor(cmd->cmd);
	if (cmd->redirects)
		free_redirects((void *)cmd->redirects);
	close_wrapper(&(cmd->backup_stdin));
	close_wrapper(&(cmd->backup_stdout));
	free(cmd);
}

void	free_redirects(void *content)
{
	t_redirect	**redirects;
	int			i;

	redirects = (t_redirect **)content;
	i = 0;
	while (redirects[i])
		free_redirect((void *)redirects[i++]);
	free(redirects);
}

void	free_redirect(void *content)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)content;
	free(redirect->token);
	free_double_pointor(redirect->file);
	free(redirect);
}
