/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:59:46 by shiori            #+#    #+#             */
/*   Updated: 2025/03/20 04:11:16 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int g_last_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_builtin	builtins[BUILTIN_NUM];
	t_list		*env;
	int			status;
	char		*line;
	t_cmd		**cmds;

	(void)argc;
	(void)argv;
	init_builtins(builtins);
	setup_interactive_signals();
	env = env_init(envp);
	if (!env)
	{
		perror("malloc");
		return (1);
	}
	while (true)
	{
		line = readline(PROMPT);
		if (!line)
		{
			ft_putstr_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			cmds = parser(line, g_last_exit_status, env);
			if (!cmds)
			{
				free(line);
				continue ;
			}
			status = execute_pipeline(cmds, builtins, env);
			if (status == -42)
			{
				free(line);
				free_cmds(cmds);
				exit(g_last_exit_status);
			}
		}
		free(line);
	}
	ft_lstclear(&env, env_free);
	return (0);
}

