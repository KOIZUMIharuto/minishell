/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:13:44 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/15 22:47:43 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cmds(t_cmd **cmds, int i)
{
	if (!cmds)
		return ;
	while (cmds[i])
		free_cmd(cmds[i++]);
	free(cmds);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_tokens(cmd->cmd);
	free_rdrcts(cmd->input_rdrct, 0);
	free_rdrcts(cmd->output_rdrct, 0);
	free(cmd);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	free_rdrcts(t_rdrct **rdrcts, int i)
{
	if (!rdrcts)
		return ;
	while (rdrcts[i])
		free_rdrct(rdrcts[i++]);
	free(rdrcts);
}

void	free_rdrct(t_rdrct *rdrct)
{
	int	i;

	if (!rdrct)
		return ;
	if (rdrct->file)
	{
		i = 0;
		while (rdrct->file[i])
			free(rdrct->file[i++]);
		free(rdrct->file);
	}
	free(rdrct);
}
