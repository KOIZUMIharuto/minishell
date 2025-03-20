/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:16:44 by shiori            #+#    #+#             */
/*   Updated: 2025/03/20 12:30:57 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int restore_redirection(t_cmd *cmd)
{
    char buffer[1024];
    struct termios term, old_term;
    
    tcgetattr(STDIN_FILENO, &old_term);
    
    term = old_term;
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    
    while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
        ;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    
    if (cmd->backup_stdin != -1)
    {
        if (dup2(cmd->backup_stdin, STDIN_FILENO) == -1)
        {
            perror("dup2");
            return -1;
        }
        close(cmd->backup_stdin);
        cmd->backup_stdin = -1;
    }
    if (cmd->backup_stdout != -1)
    {
        if (dup2(cmd->backup_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            return -1;
        }
        close(cmd->backup_stdout);
        cmd->backup_stdout = -1;
    }
    return 0;
}

int    handle_ambiguous_rdrct(t_rdrct *rdrct)
{
    (void)rdrct;
    ft_putstr_fd(rdrct->token, STDERR_FILENO);
    ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
    return (-1);
}
