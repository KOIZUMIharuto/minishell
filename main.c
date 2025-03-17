/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:59:46 by shiori            #+#    #+#             */
/*   Updated: 2025/03/17 14:14:06 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int g_last_exit_status = 0;

// hkoizumi: (t_cmd **, t_builtin *, t_list *)を受け取るようにしたい
// int interpret_line(char *line, t_builtin *builtins, t_list *env)
// {
// 	// hkoizumi: ここら辺のsplitとかはt_cmdを受け取った時点で解析済みなので不要
//     char **tokens = ft_split(line, '|'); // パイプで分割
//     if (tokens == NULL || tokens[0] == NULL) {
//         free(tokens);
//         return (0);
//     }
//     int status;
//     if (tokens[0] != NULL) {
// 		// hkoizumi: pipe_commandもchar **の代わりにt_cmd **を渡すようにしたい
//         status = pipe_command(tokens, builtins, env);
//     }
//     for (int i = 0; tokens[i] != NULL; i++) {
//         free(tokens[i]);
//     }
//     free(tokens);

//     if (WIFEXITED(status)) {
//         g_last_exit_status = WEXITSTATUS(status);
//     } else if (WIFSIGNALED(status)) {
//         g_last_exit_status = 128 + WTERMSIG(status);
//     }

//     return (status);
// }

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
			write(1, "exit\n", 5);
			break ;
		}
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			cmds = purser(line, g_last_exit_status, env);
			status = execute_pipeline(cmds, builtins, env);	// hkoizumi: ここで(cmds, builtins, env)を渡すようにしたい
			if (status == -42)
			{
				free(line);
				free_cmds(cmds, 0);
				exit(g_last_exit_status);
			}
		}
		free(line);
		free_cmds(cmds, 0);
	}
	ft_lstclear(&env, env_free);
	return (0);
}

