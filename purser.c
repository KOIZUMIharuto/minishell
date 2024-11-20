/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/21 01:43:31 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "purser.h"
#include "libft/get_next_line.h"	// テスト用

static t_cmd			**recursive_purser(char **tokens, int cmd_cnt);
static t_redirection	**check_redirection(char *line, char key, int redirect_cnt);
static int				set_redirection_type(char *key_pos, char key, t_redirection *redirection);
static char				**recursive_tokenize(char *line, int word_cnt);
static bool				is_whitespace(char c, t_quote *quote);

t_cmd	**purser(char *line)
{
	t_cmd	**cmds;
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = ft_split(line, '|');
	if (!tokens)
		return (NULL);
	cmds = recursive_purser(tokens, 0);
	return (cmds);
}

static t_cmd	**recursive_purser(char **tokens, int cmd_cnt)
{
	t_cmd	**cmds;
	t_cmd	*cmd;
	int		i;

	if (!tokens[cmd_cnt])
	{
		cmds = (t_cmd **)ft_calloc(cmd_cnt + 1, sizeof(t_cmd *));
		if (!cmds)
			return (NULL);
	}
	else
	{
		cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!cmd)
			return (NULL);
		cmd->input_redurection = check_redirection(tokens[cmd_cnt], '<', 0);
		cmd->output_redurection = check_redirection(tokens[cmd_cnt], '>', 0);
		cmd->cmd = recursive_tokenize(tokens[cmd_cnt], 0);
		cmd->infile_fd = -1;
		cmd->outfile_fd = -1;
		if (!cmd->input_redurection || !cmd->output_redurection || !cmd->cmd)
		{
			free_cmd(cmd);
			return (NULL);
		}
		cmds = recursive_purser(tokens, cmd_cnt + 1);
		if (!cmds)
		{
			free_cmd(cmd);
			return (NULL);
		}
		cmds[cmd_cnt] = cmd;	
	}
	return (cmds);
}

static t_redirection	**check_redirection(char *line, char key, int redirect_cnt)
{
	t_redirection		**redirections;
	t_redirection		*redirection;
	char				*key_pos;
	char				*key_pos_tmp;
	int					i;

	if (!line)
		return (NULL);
	key_pos = ft_strchr(line, key);
	if (!key_pos)
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
		key_pos += set_redirection_type(key_pos, key, redirection);
		while (*key_pos && ft_strchr(" \t", *key_pos))
			key_pos++;
		i = 0;
		while (key_pos[i] && !ft_strchr(" \t\n<>", key_pos[i]))
			i++;
		redirection->file = ft_substr(key_pos, 0, i);
		redirection->fd = -1;
		if (!redirection->file)
		{
			free(redirection);
			return (NULL);
		}
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

static char	**recursive_tokenize(char *line, int word_cnt)
{
	char	**tokens;
	char	*token;
	t_quote	quote;
	int		i;

	while (*line && is_whitespace(*line, NULL))
		line++;
	if (!*line)
	{
		tokens = (char **)ft_calloc(word_cnt + 1, sizeof(char *));
	}
	else
	{
		quote = (t_quote){false, false};
		i = 0;
		while (line[i] && !is_whitespace(line[i], &quote))
			i++;
		token = ft_substr(line, 0, i);
		if (!token)
			return (NULL);
		tokens = recursive_tokenize(line + i, word_cnt + 1);
		if (!tokens)
			free(token);
		else
			tokens[word_cnt] = token;
	}
	return (tokens);
}

static bool	is_whitespace(char c, t_quote *quote)
{
	if (quote)
	{
		if (c == '"')
			quote->double_quote = !quote->double_quote;
		else if (c == '\'')
			quote->single_quote = !quote->single_quote;
		if (quote->single_quote || quote->double_quote)
			return (false);
	}
	return (c == ' ' || c <= '\t' || c == '\n');
}


// テスト用

static void print_cmds(t_cmd **cmds);

int main()
{
	char *line;
	t_cmd **cmds;

	while (1)
	{
		write(1, "purser$ ", 8);
		line = get_next_line(0);
		if (!line)
			break;
		cmds = purser(line);
		if (!cmds)
		{
			free(line);
			break;
		}
		print_cmds(cmds);
		free_cmds(cmds);
		free(line);
	}
	return (0);
}

static void print_cmds(t_cmd **cmds)
{
	if (!cmds)
		return ;
	for (int i = 0; cmds[i]; i++)
	{
		printf("cmds[%d]:\n", i);
		printf("\tcmd: ");
		for (int j = 0; cmds[i]->cmd[j]; j++)
		{
			if (j != 0)
				printf(" ");
			printf("[%s]", cmds[i]->cmd[j]);
		}
		printf("\n\ttinput_redurection\n");
		if (!cmds[i]->input_redurection[0])
			printf("\t\t[0]: <NULL>(type: NONE_REDIRECTION)\n");
		for (int j = 0; cmds[i]->input_redurection[j]; j++)
		{
			printf("\t\t[%d]: <%s>", j, cmds[i]->input_redurection[j]->file);
			if (cmds[i]->input_redurection[j]->type == INPUT_REDIRECTION)
				printf("(type: INPUT_REDIRECTION)\n");
			else
				printf("(type: HEREDOCUMENT)\n");
		}
		printf("\toutput_redurection\n");
		if (!cmds[i]->output_redurection[0])
			printf("\t\t[0]: <NULL>(type: NONE_REDIRECTION)\n");
		for (int j = 0; cmds[i]->output_redurection[j]; j++)
		{
			printf("\t\t[%d]: <%s>", j, cmds[i]->output_redurection[j]->file);
			if (cmds[i]->output_redurection[j]->type == OVERWRITE_REDIRECTION)
				printf("(type: OVERWRITE_REDIRECTION)\n");
			else
				printf("(type: APPEND_REDIRECTION)\n");
		}
	}
}

