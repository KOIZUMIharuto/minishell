/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:21 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/19 02:51:38 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int g_last_exit_status = 0;

// t_cmd	**parser(char *line, int exit_status, t_list *env)
// {

// }

// トークンリストを出力する (デバッグ用)
void	print_tokens(t_list *tokens)
{
	int i = 0;
	while (tokens)
	{
		printf("Token %d: [%s]\n", i++, (char *)tokens->content);
		tokens = tokens->next;
	}
}

// readline を使った入力テスト
int	main(void)
{
	char	*input;
	t_list	*tokens;
	t_list	**cmds;
	int		i;

	while (1)
	{
		input = readline("minishell> ");
		if (!input) // EOF (Ctrl+D)
		{
			printf("exit\n");
			break ;
		}
		if (*input) // 空行でない場合は履歴に追加
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
			printf("Tokenize error\n");
		else
		{
			print_tokens(tokens);
			if (!check_syntax(tokens))
				ft_lstclear(&tokens, free);
			else
			{
				printf("Syntax OK\n");
				cmds = split_tokens(tokens);
				if (!cmds)
				{
					printf("Split error\n");
					ft_lstclear(&tokens, free);
				}
				else
				{
					i = 0;
					while (cmds[i])
					{
						printf("Command %d:\n", i);
						print_tokens(cmds[i]);
						ft_lstclear(&cmds[i], free);
						i++;
					}
					free(cmds);
				}
			}
		}
		free(input);
	}
	return (0);
}
