/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:08:45 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/24 12:50:30 by hkoizumi         ###   ########.fr       */
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
	if (cmd->rdrcts)
		free_rdrcts((void *)cmd->rdrcts);
	if (cmd->backup_stdin != -1)
		close(cmd->backup_stdin);
	if (cmd->backup_stdout != -1)
		close(cmd->backup_stdout);
	free(cmd);
}

void	free_rdrcts(void *content)
{
	t_rdrct	**rdrcts;
	int		i;

	rdrcts = (t_rdrct **)content;
	i = 0;
	while (rdrcts[i])
		free_rdrct((void *)rdrcts[i++]);
	free(rdrcts);
}

void	free_rdrct(void *content)
{
	t_rdrct	*rdrct;

	rdrct = (t_rdrct *)content;
	free(rdrct->token);
	free_double_pointor(rdrct->file);
	free(rdrct);
}
