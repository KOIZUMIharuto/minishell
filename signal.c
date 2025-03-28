/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:01:52 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 17:55:52 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
 * SIGINT (Ctrl+C) のインタラクティブモード時のハンドラ
 */
void sig_ctrl_c(int signal)
{
    (void)signal;
    rl_on_new_line();
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_redisplay();
    g_last_exit_status = 130;
}


/**
 * SIGINT (Ctrl+C) の外部コマンド実行中のハンドラ
 */
void sig_ctrl_c_exec(int signal)
{
    (void)signal;
    write(STDOUT_FILENO, "^C\n", 3);
    g_last_exit_status = 130;  // Ctrl+C で中断時
}

/**
 * インタラクティブモードのシグナル設定
 * - SIGINT → `sig_ctrl_c`
 * - SIGQUIT → 無視
 */
void setup_interactive_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
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

/**
 * ビルトインコマンド実行時のシグナル設定
 * - SIGINT, SIGQUIT → 無視
 */
void setup_builtin_signals(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

/**
 * 外部コマンド実行中のシグナル設定
 * - SIGINT → `sig_ctrl_c_exec`
 * - SIGQUIT → 無視
 */
void setup_exec_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    // sa.sa_handler = SIG_IGN;
    sa.sa_handler = sig_ctrl_c_exec;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

/**
 * 子プロセスのシグナル設定
 * - SIGINT, SIGQUIT → デフォルト
 */
void setup_child_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

/**
 * ヒアドキュメントのシグナル設定
 * - SIGINT → デフォルト
 * - SIGQUIT → 無視
 */
void setup_heredoc_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
    tcgetattr(STDIN_FILENO, &term);

    // term.c_lflag |= ECHOCTL;
    // tcsetattr(STDIN_FILENO, TCSANOW, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_after_rl_signals(void)
{
    struct sigaction sa;
    struct termios term;

    ft_bzero(&sa, sizeof(sa));
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
