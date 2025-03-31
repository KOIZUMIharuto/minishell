/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:01:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/31 23:42:01 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	sig_ctrl_c_exec(int signal);
static void	sig_ctrl_c(int signal);

void	setup_after_rl_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&term, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	sa.sa_handler = sig_ctrl_c_exec;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_exec_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&term, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	sa.sa_handler = sig_ctrl_c_exec;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static void	sig_ctrl_c_exec(int signal)
{
	(void)signal;
	print_msg("^C\n", STDOUT_FILENO);
	g_last_exit_status = 130;
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&term, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	sa.sa_handler = sig_ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static void	sig_ctrl_c(int signal)
{
	(void)signal;
	rl_on_new_line();
	print_msg("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_redisplay();
	g_last_exit_status = 130;
}
