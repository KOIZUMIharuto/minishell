/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:59:46 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 17:43:14 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_valid	prompt(t_builtin *builtins, t_list *env);

int	g_last_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_builtin	builtins[BUILTIN_NUM];
	t_list		*env;
	t_valid		status;

	(void)argc;
	(void)argv;
	init_builtins(builtins);
	env = env_init(envp);
	if (!env)
		return (perror_int("malloc"));
	while (true)
	{
		status = prompt(builtins, env);
		if (status == ERROR)
			break ;
	}
	ft_lstclear(&env, env_free);
	return (g_last_exit_status);
}

static t_valid	prompt(t_builtin *builtins, t_list *env)
{
	char	*line;
	t_valid	valid_status;
	t_cmd	**cmds;

	setup_interactive_signals();
	line = readline(PROMPT);

	if (!line)
	{
		if (!print_msg("exit\n", STDOUT_FILENO))
			g_last_exit_status = EXIT_FAILURE;
		return (ERROR);
	}
	if (ft_strlen(line) > 0)
	{
		setup_after_rl_signals();
		add_history(line);
		valid_status = parser(&cmds, line, g_last_exit_status, env);
		free(line);
		if (valid_status != VALID)
			return (valid_status);
		if (execute_pipeline(cmds, builtins, env) == -42)
			return (ERROR);
	}
	else
		free(line);
	return (VALID);
}
