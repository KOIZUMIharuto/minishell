/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:01:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 12:13:08 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


void sig_ctrl_c(int signal)
{
    (void)signal; 
    write(1, "\n", 1);
    rl_on_new_line();
    // rl_replace_line("", 0);
    rl_redisplay();
}

void sig_ctrl_c_exec(int signal)
{
    (void)signal; 
    g_last_exit_status = 130;
    write(1, "\n", 1);
}

void setup_interactive_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sig_ctrl_c;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_builtin_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_exec_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sig_ctrl_c_exec;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_child_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}
