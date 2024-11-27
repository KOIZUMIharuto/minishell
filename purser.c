/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/27 17:18:27 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>
#include <get_next_line.h>

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
		printf("\targs:\n");
		t_list *tmp = cmds[i]->args;
		while (1)
		{
			if (!tmp)
			{
				printf("\t\t(null)\n");
				break;
			}
			printf("\t\t%s\n", (char *)tmp->content);
			tmp = tmp->next;
		}
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
			printf("\tfile: <%s>\n", cmds[i]->input_redurection[j]->file);
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
			printf("\tfile: <%s>\n", cmds[i]->output_redurection[j]->file);
			if (cmds[i]->output_redurection[j]->type == OVERWRITE_REDIRECTION)
				printf("\t\t\ttype: OVERWRITE_REDIRECTION\n");
			else
				printf("\t\t\ttype: APPEND_REDIRECTION\n");
		}
	}
}

// // leak check destractor
// void __attribute__((destructor)) destructor(void)
// {
// 	system("leaks purser");
// }