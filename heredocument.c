/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 23:05:10 by shiori            #+#    #+#             */
/*   Updated: 2025/03/27 16:56:38 by shiori           ###   ########.fr       */
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
			line++;
			if (*line == '?')  // `$?`
			{
				ft_putnbr_fd(g_last_exit_status, tmp_fd);
				line++;
				continue;
			}
			else if (ft_isalpha(*line) || *line == '_') // `$VAR`
			{
				list_tmp = env_list;
				while (list_tmp)
				{
					env = (t_env *)list_tmp->content;
					key_len = ft_strlen(env->key);
					if (ft_strncmp(env->key, line, key_len) == 0 &&
						!(ft_isalnum(line[key_len]) || line[key_len] == '_'))
					{
						if (write(tmp_fd, env->value, ft_strlen(env->value)) == -1)
							return (-1);
						line += key_len;
						break;
					}
					list_tmp = list_tmp->next;
				}
				if (list_tmp)
					continue;
				// 未定義環境変数（$XYZなど）をスキップ
				while (ft_isalnum(*line) || *line == '_')
					line++;
				continue;
			}
			else
			{
				// $のあとが ? でもアルファベットでもない → $だけ出力
				if (write(tmp_fd, "$", 1) == -1)
					return (-1);
				continue;
			}
		}
		// 通常文字の出力
		if (write(tmp_fd, line, 1) == -1)
			return (-1);
		line++;
	}
	return (0);
}

// static int	write_expand_env(int tmp_fd, char *line, t_list *env_list)
// {
// 	t_list	*list_tmp;
// 	t_env	*env;
// 	int		key_len;

//     while (*line)
//     {
// 		if (*line == '$')
// 		{
// 			if (line[1] == '?')
// 			{
// 				ft_putnbr_fd(g_last_exit_status, tmp_fd);
// 				line += 2;
// 				continue ;
// 			}
// 			else if (ft_isalpha(line[1]) || line[1] == '_')
// 			{
// 				line++;
// 				list_tmp = env_list;
// 				while (list_tmp)
// 				{
// 					env = (t_env *)list_tmp->content;
// 					key_len = ft_strlen(env->key);
// 					if (ft_strncmp(env->key, line, key_len) == 0
// 						&& !(ft_isalnum(line[key_len]) || line[key_len] == '_'))
// 					{
// 						if (write(tmp_fd, env->value, ft_strlen(env->value)) == -1)
// 							return (-1);
// 						line += key_len;
// 						break ;
// 					}
// 					list_tmp = list_tmp->next;
// 				}
// 				if (list_tmp)
// 					continue ;
// 				while (ft_isalnum(*line) || *line == '_')
// 					line++;
// 				if (write(tmp_fd, "", 0) == -1)
// 					return (-1);
// 				continue ;
// 			}
// 		}
// 		if (write(tmp_fd, line++, 1) == -1)
// 			return (-1);
// 	}
// 	return (0);
// }
static int setup_heredoc_pipe(int pipe_fds[2], t_cmd *cmd)
{
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (cmd->backup_stdin == -1)
    {
        cmd->backup_stdin = dup(STDIN_FILENO);
        if (cmd->backup_stdin == -1)
        {
            perror("dup");
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            return (-1);
        }
    }
    else
    {
        dup2(cmd->backup_stdin, STDIN_FILENO);
    }
    return (0);
}

static void	process_heredoc_child(int pipe_fds[2], t_rdrct *rdrct, t_list* env)
{
	char	*line;
	char	*delimiter;

	delimiter = rdrct->file[0];

	close(pipe_fds[0]);
    setup_heredoc_signals();


    while (1)
	{
		line = readline("> ");
		
		// EOFチェック
		if (!line) {
            exit(0);
        }

        if (g_last_exit_status == 130)
                exit(130);
		
		// デリミタチェック
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// 内容書き込み
		if (rdrct->is_quoted)
		{
			write(pipe_fds[1], line, ft_strlen(line));
			write(pipe_fds[1], "\n", 1);
		}
		else
		{
			if (ft_strlen(line) > 0)
				write_expand_env(pipe_fds[1], line, env);
			write(pipe_fds[1], "\n", 1);
		}
		free(line);
	}
	close(pipe_fds[1]);
	exit(0);
}

static int	setup_parent_process(int pipe_fds[2], t_cmd *cmd, pid_t pid)
{
    int status;
    
    waitpid(pid, &status, 0);
    close(pipe_fds[1]);
    if (WIFSIGNALED(status)) {
       g_last_exit_status = WTERMSIG(status) + 128;
       close(pipe_fds[0]);
       printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
       return (-1);  // シグナルで終了した場合は常に処理中断
   }
	if ((WIFEXITED(status)))
    {
        g_last_exit_status = WEXITSTATUS(status);
        if(WEXITSTATUS(status) != 0)
        {   
            close(pipe_fds[0]);
            return (-1);
        }
    }
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
		close(pipe_fds[0]);
		return (-1);
	}
	close(pipe_fds[0]);
    printf("*************************************\n");
    printf("backup_stdin: %d\n", cmd->backup_stdin);
    printf("*************************************\n");

	return (0);
}

int	handle_heredocument(t_rdrct *rdrct, t_cmd *cmd, t_list* env)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (setup_heredoc_pipe(pipe_fds, cmd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(cmd->backup_stdin);
		cmd->backup_stdin = -1;
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	if (pid == 0)
		process_heredoc_child(pipe_fds, rdrct, env);
	return(setup_parent_process(pipe_fds, cmd, pid));
}
	