/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:07:49 by shiori            #+#    #+#             */
/*   Updated: 2025/03/28 20:05:18 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <minishell.h>


static int	write_expand_env(int tmp_fd, char *line, t_list *env_list)
{
	t_list	*list_tmp;
	t_env	*env;
	int		key_len;

    while (*line)
    {
		if (*line == '$')
		{
			if (line[1] == '?')
			{
				ft_putnbr_fd(g_last_exit_status, tmp_fd);
				line += 2;
				continue ;
			}
			else if (ft_isalpha(line[1]) || line[1] == '_')
			{
				line++;
				list_tmp = env_list;
				while (list_tmp)
				{
					env = (t_env *)list_tmp->content;
					key_len = ft_strlen(env->key);
					if (ft_strncmp(env->key, line, key_len) == 0
						&& !(ft_isalnum(line[key_len]) || line[key_len] == '_'))
					{
						if (write(tmp_fd, env->value, ft_strlen(env->value)) == -1)
							return (-1);
						line += key_len;
						break ;
					}
					list_tmp = list_tmp->next;
				}
				if (list_tmp)
					continue ;
				while (ft_isalnum(*line) || *line == '_')
					line++;
				if (write(tmp_fd, "", 0) == -1)
					return (-1);
				continue ;
			}
		}
		if (write(tmp_fd, line++, 1) == -1)
			return (-1);
	}
	return (0);
}

static t_valid setup_heredoc_pipe(int pipe_fds[2], t_cmd *cmd)
{
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return (CRITICAL_ERROR);
    }
    if (cmd->backup_stdin == -1)
    {
        cmd->backup_stdin = dup(STDIN_FILENO);
        if (cmd->backup_stdin == -1)
        {
            perror("dup");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            return (CRITICAL_ERROR);
        }
    }
    else
    {
        if(dup2(cmd->backup_stdin, STDIN_FILENO) == -1) 
        {
            perror("dup2");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            return (CRITICAL_ERROR);
        }
    }
    return (VALID);
}

static void	process_heredoc_child(int pipe_fds[2], t_rdrct *rdrct, t_list *env)
{
    char	*line;
    char	*delimiter;

	delimiter = rdrct->file[0];
	close(pipe_fds[0]);
	setup_heredoc_signals();

    while (1)
    {
        line = readline("> ");

        // ✅ EOF (Ctrl+D) で終了
        if (!line)
        {
            close(pipe_fds[1]);
            exit(0);
        }

        // ✅ デリミタチェック
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

		if ((!rdrct->is_quoted && ft_strlen(line) > 0
				&& write_expand_env(pipe_fds[1], line, env) == -1)
			|| write(pipe_fds[1], line, ft_strlen(line)) == -1)
		{
			perror("write");
			free(line);
			exit(1);
		}
		if (write(pipe_fds[1], "\n", 1) == -1)
		{
			perror("write");
			free(line);
			exit (1);
		}
        free(line);
    }

    close(pipe_fds[1]);
    exit(VALID);
}

t_valid	setup_parent_process(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{

    int status;
    pid_t result;

    close(pipe_fds[1]);  // ✅ 書き込みパイプを閉じる

    // ✅ EINTR の場合は再試行
	// printf("wait start\n");
	result = waitpid(pid, &status, 0);
	while (result == -1 && errno == EINTR)
        result = waitpid(pid, &status, 0);

    if (result == -1)
    {
        perror("waitpid");
        close(pipe_fds[0]);
        return (CRITICAL_ERROR);
    }

    // ✅ シグナルで中断された場合
    if (WIFSIGNALED(status))
    {
        g_last_exit_status = WTERMSIG(status) + 128;
        close(pipe_fds[0]);
        if (cmd->backup_stdin != -1)
        {
            if(dup2(cmd->backup_stdin, STDIN_FILENO)==-1);
            {
                perror("dup2");
                close(cmd->backup_stdin);
                return (CRITICAL_ERROR);
            }
            close(cmd->backup_stdin);
            cmd->backup_stdin = -1;
        }
        return (SIGINT_EXIT); // ✅ 中断時は -42 を返す
    }

    // ✅ 正常終了の場合
    if (WIFEXITED(status))
    {
        g_last_exit_status = WEXITSTATUS(status);
        if (g_last_exit_status != 0)
        {
            
            close(pipe_fds[0]); 
            return (CRITICAL_ERROR); //koko
        }
    }

    // ✅ ヒアドキュメントの内容を標準入力にリダイレクト
    if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(pipe_fds[0]);
        return (CRITICAL_ERROR);
    }
    close(pipe_fds[0]);
    return (VALID);
}

t_valid process_heredocs(t_cmd *cmd, t_list *env)
{
	int		i;
    int pipe_fds[2];
    pid_t pid;
    int result;

	i = -1;
	while (cmd->rdrcts[++i])
	{
		if (cmd->rdrcts[i]->type != HEREDOCUMENT)
			continue ;
		if (setup_heredoc_pipe(pipe_fds, cmd) == CRITICAL_ERROR)
			return (CRITICAL_ERROR);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			return (CRITICAL_ERROR);
		}
		if (pid == 0)
		{
			if (cmd->backup_stdin != -1)
				close(cmd->backup_stdin);
			process_heredoc_child(pipe_fds, cmd->rdrcts[i], env);
		}
		result = setup_parent_process(pipe_fds, cmd, pid);
		setup_interactive_signals();  
		if (result == SIGINT_EXIT)
		{
			if(dup2(cmd->backup_stdin, STDIN_FILENO)==-1)
			{
				perror("dup2");
				close(cmd->backup_stdin);
				return (CRITICAL_ERROR);
			}
			close(cmd->backup_stdin);
			cmd->backup_stdin = -1;
			return(SIGINT_EXIT); //koko
		}
	}
    return (VALID);
}
