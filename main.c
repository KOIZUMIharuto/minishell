/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:59:46 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 13:09:01 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


static t_valid	prompt(t_builtin *builtins, t_list *env);
static bool		is_skip_line(char *line);

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
		if (status != VALID)
			break ;
	}
	ft_lstclear(&env, env_free);
	return (g_last_exit_status);
}

static t_valid	prompt(t_builtin *builtins, t_list *env)
{
	char	*line;
	t_valid	is_valid;
	t_cmd	**cmds;

	setup_interactive_signals();
	line = readline(PROMPT);
	if (!line)
	{
		if (!print_msg("exit\n", STDOUT_FILENO))
			g_last_exit_status = EXIT_FAILURE;
		return (CRITICAL_ERROR);
	}
	if (!is_skip_line(line))
	{
		setup_after_rl_signals();
		add_history(line);
		is_valid = parser(&cmds, line, g_last_exit_status, env);
		free(line);
		if (is_valid != VALID)
			return (is_valid);
		is_valid = execute_pipeline(cmds, builtins, env);
		if (is_valid == EXIT_MAINT_LOOP || is_valid == CRITICAL_ERROR)
			return (is_valid);
	}
	else
		free(line);
	return (VALID);
}

static bool	is_skip_line(char *line)
{
	if (ft_strlen(line) == 0)
		return (true);
	while (*line)
	{
		if (!ft_strchr(" \t\n\v\f\r", *line))
			return (false);
		line++;
	}
	return (true);
}
