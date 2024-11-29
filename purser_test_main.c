/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_test_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:43:58 by hkoizumi          #+#    #+#             */
/*   Updated: 2024/11/29 15:44:18 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <purser.h>

static void	print_cmds(t_cmd **cmds);

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_cmd	**cmds;

	(void)argc;
	(void)argv;
	environ = env;
	while (1)
	{
		write(1, "purser$ ", 8);
		line = get_next_line(0);
		if (!line)
			break ;
		cmds = purser(line);
		if (!cmds)
		{
			free(line);
			break ;
		}
		print_cmds(cmds);
		free_cmds(cmds, 0);
		free(line);
	}
	return (0);
}

static void	print_cmds(t_cmd **cmds)
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
		printf("\n\tinput_rdrct\n");
		if (!cmds[i]->input_rdrct[0])
			printf("\t\t[0]: \t(null)\n");
		for (int j = 0; cmds[i]->input_rdrct[j]; j++)
		{
			printf("\t\t[%d]:", j);
			printf("\tfile: <");
			if (!cmds[i]->input_rdrct[j]->file)
				printf("(null)>\n");
			else
			{
				for (int k = 0; cmds[i]->input_rdrct[j]->file[k]; k++)
					printf("[%s]", cmds[i]->input_rdrct[j]->file[k]);
				printf(">\n");
			}
			if (cmds[i]->input_rdrct[j]->type == INPUT_rdrct)
				printf("\t\t\ttype: INPUT_rdrct\n");
			else
				printf("\t\t\ttype: HEREDOCUMENT\n");
		}
		printf("\toutput_rdrct\n");
		if (!cmds[i]->output_rdrct[0])
			printf("\t\t[0]: \t(null)\n");
		for (int j = 0; cmds[i]->output_rdrct[j]; j++)
		{
			printf("\t\t[%d]:", j);
			printf("\tfile: <");
			if (!cmds[i]->output_rdrct[j]->file)
				printf("(null)>\n");
			else
			{
				for (int k = 0; cmds[i]->output_rdrct[j]->file[k]; k++)
					printf("[%s]", cmds[i]->output_rdrct[j]->file[k]);
				printf(">\n");
			}
			if (cmds[i]->output_rdrct[j]->type == OVERWRITE_rdrct)
				printf("\t\t\ttype: OVERWRITE_rdrct\n");
			else
				printf("\t\t\ttype: APPEND_rdrct\n");
		}
	}
}

// __attribute__((destructor))
// static void destructor() {
// 	system("leaks -q purser");
// }
