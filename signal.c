/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:01:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/26 03:24:47 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void sig_ctrl_c(int signal)
{
    (void)signal;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_last_exit_status = 1;
}

void sig_ctrl_c_exec(int signal)
{
    (void)signal; 
    g_last_exit_status = 130;
    write(1, "\n", 1);
}

void setup_interactive_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&term, sizeof(term));
    // 現在の端末設定を取得
    tcgetattr(STDIN_FILENO, &term);
    
    // ECHOCTL フラグをオフにして、^C などの制御文字が表示されないようにする
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    
    sa.sa_handler = sig_ctrl_c;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // SA_RESTART フラグがないことを確認
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_builtin_signals(void)
{
    struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_exec_signals(void)
{
    struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = sig_ctrl_c_exec;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_child_signals(void)
{
    struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
    ft_bzero(&term, sizeof(term));

    // 現在の端末設定を取得
    tcgetattr(STDIN_FILENO, &term);
    
    // `ECHOCTL` をオフにして `^C` の表示を抑制
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // `SIGINT` (`Ctrl+C`) はデフォルトの動作を適用 (中断可能にする)
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // `SIGQUIT` (`Ctrl+\`) は無視する
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}




