/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   purser_test_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:43:58 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 13:27:35 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int g_last_exit_status = 0;

static void	print_cmds(t_cmd **cmds);

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_cmd	**cmds;
	t_list	*env_list;

	if (argc > 1)
		g_last_exit_status = ft_atoi(argv[1]);
	env_list = env_init(env);
	if (!env_list)
		return (1);
	while (1)
	{
		line = readline("purser$ ");
		if (!line)
			break ;
		cmds = purser(line, g_last_exit_status, env_list);
		free(line);
		if (!cmds)
			return (perror_int("malloc", errno));
		print_cmds(cmds);
		free_cmds(cmds, 0);
	}
	ft_lstclear(&env_list, env_free);
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
		printf("\n\tINPUT_RDRCT\n");
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
			if (cmds[i]->input_rdrct[j]->type == INPUT_RDRCT)
				printf("\t\t\ttype: INPUT_RDRCT\n");
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
			if (cmds[i]->output_rdrct[j]->type == OVERWRITE_RDRCT)
				printf("\t\t\ttype: OVERWRITE_RDRCT\n");
			else
				printf("\t\t\ttype: APPEND_RDRCT\n");
		}
	}
}

// __attribute__((destructor))
// static void destructor() {
// 	system("leaks -q purser");
// }
