/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purse_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:13:44 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/20 21:24:29 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "purser.h"

void	free_cmds(t_cmd **cmds)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
		free_cmd(cmds[i++]);
	free(cmds);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_tokens(cmd->cmd);
	free_redirections(cmd->input_redurection);
	free_redirections(cmd->output_redurection);
	free(cmd);
}

void	free_redirections(t_redirection **redirections)
{
	int	i;

	if (!redirections)
		return ;
	i = 0;
	while (redirections[i])
		free_redirection(redirections[i++]);
	free(redirections);
}

void	free_redirection(t_redirection *redirection)
{
	if (!redirection)
		return ;
	free(redirection->file);
	free(redirection);
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