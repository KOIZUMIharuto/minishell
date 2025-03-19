/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 22:48:05 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int g_last_exit_status = 0;

static void	print_cmds(t_cmd **cmds);

// readline を使った入力テスト
int	main(int argc, char **argv, char **envp)
{
	t_list	*env;
	char	*input;
	t_cmd	**cmds;

	(void)argc;
	(void)argv;
	env = env_init(envp);
	if (!env)
		return (1);
	while (1)
	{
		input = readline("parser> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		cmds = parser(input, g_last_exit_status, env);
		free(input);
		if (!cmds)
			continue ;
		print_cmds(cmds);
		for (int i = 0; cmds[i]; i++)
			free_cmd((void *)cmds[i]);
		free(cmds);
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
		printf("\n\tINPUT_REDIRECT\n");
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
			if (cmds[i]->input_rdrct[j]->type == INPUT_REDIRECT)
				printf("\t\t\ttype: INPUT_REDIRECT\n");
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
			if (cmds[i]->output_rdrct[j]->type == OVERWRITE_REDIRECT)
				printf("\t\t\ttype: OVERWRITE_REDIRECT\n");
			else
				printf("\t\t\ttype: APPEND_REDIRECT\n");
		}
	}
}

// leaks check
__attribute__((destructor))
void	destructor(void)
{
	system("leaks parser");
}
