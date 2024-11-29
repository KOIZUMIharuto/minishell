/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/29 12:21:22 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static char		**recursive_split(char *line, char *del, int word_cnt);
static t_cmd	**recursive_purser(char **tokens, int cmd_cnt);

t_cmd	**purser(char *line)
{
	t_cmd	**cmds;
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = recursive_split(line, "|", 0);
	if (!tokens)
		return (NULL);
	cmds = recursive_purser(tokens, 0);
	free_tokens(tokens);
	return (cmds);
}

static char	**recursive_split(char *line, char *del, int word_cnt)
{
	char	**tokens;
	char	*token;
	t_quote	quote;
	int		i;

	while (*line && is_del(*line, del, NULL))
		line++;
	if (!*line)
	{
		tokens = (char **)ft_calloc(word_cnt + 1, sizeof(char *));
	}
	else
	{
		quote = NONE_QUOTE;
		i = 0;
		while (line[i] && !is_del(line[i], del, &quote))
			i++;
		token = ft_substr(line, 0, i);
		if (!token)
			return (NULL);
		tokens = recursive_split(line + i, del, word_cnt + 1);
		if (!tokens)
			free(token);
		else
			tokens[word_cnt] = token;
	}
	return (tokens);
}

static t_cmd	**recursive_purser(char **tokens, int cmd_cnt)
{
	t_cmd	**cmds;
	t_cmd	*cmd;

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
		cmd->input_redurection = check_redirection(tokens[cmd_cnt], "<", 0);
		cmd->output_redurection = check_redirection(tokens[cmd_cnt], ">", 0);
		cmd->cmd = split_arg(tokens[cmd_cnt]);
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

bool	is_del(char c, char *del, t_quote *quote)
{
	bool	del_quote;

	if (ft_strchr(del, '"') && ft_strchr(del, '\''))
		del_quote = true;
	else
		del_quote = false;
	if (quote)
	{
		if (c == '"')
		{
			if (*quote == DOUBLE_QUOTE)
				*quote = NONE_QUOTE;
			else if (*quote == NONE_QUOTE)
				*quote = DOUBLE_QUOTE;
		}
		else if (c == '\'')
		{
			if (*quote == SINGLE_QUOTE)
				*quote = NONE_QUOTE;
			else if (*quote == NONE_QUOTE)
				*quote = SINGLE_QUOTE;
		}
		if (!del_quote && *quote != NONE_QUOTE)
			return (false);
		if ((*quote == SINGLE_QUOTE && c == '"') || (*quote == DOUBLE_QUOTE && c == '\''))
			return (false);
	}
	return (ft_strchr(del, c) != NULL);
}


// テスト用

static void print_cmds(t_cmd **cmds);

int main(int argc, char **argv, char **env)
{
	char *line;
	t_cmd **cmds;

	(void)argc;
	(void)argv;
	environ = env;
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
		printf("\n\tinput_redurection\n");
		if (!cmds[i]->input_redurection[0])
			printf("\t\t[0]: \t(null)\n");
		for (int j = 0; cmds[i]->input_redurection[j]; j++)
		{
			printf("\t\t[%d]:", j);
			printf("\tfile: <");
			if (!cmds[i]->input_redurection[j]->file)
				printf("(null)>\n");
			else
			{
				for (int k = 0; cmds[i]->input_redurection[j]->file[k]; k++)
					printf("[%s]", cmds[i]->input_redurection[j]->file[k]);
				printf(">\n");
			}
			if (cmds[i]->input_redurection[j]->type == INPUT_REDIRECTION)
				printf("\t\t\ttype: INPUT_REDIRECTION\n");
			else
				printf("\t\t\ttype: HEREDOCUMENT\n");
		}
		printf("\toutput_redurection\n");
		if (!cmds[i]->output_redurection[0])
			printf("\t\t[0]: \t(null)\n");
		for (int j = 0; cmds[i]->output_redurection[j]; j++)
		{
			printf("\t\t[%d]:", j);
			printf("\tfile: <");
			if (!cmds[i]->output_redurection[j]->file)
				printf("(null)>\n");
			else
			{
				for (int k = 0; cmds[i]->output_redurection[j]->file[k]; k++)
					printf("[%s]", cmds[i]->output_redurection[j]->file[k]);
				printf(">\n");
			}
			if (cmds[i]->output_redurection[j]->type == OVERWRITE_REDIRECTION)
				printf("\t\t\ttype: OVERWRITE_REDIRECTION\n");
			else
				printf("\t\t\ttype: APPEND_REDIRECTION\n");
		}
	}
}

// __attribute__((destructor))
// static void destructor() {
// 	system("leaks -q purser");
// }
