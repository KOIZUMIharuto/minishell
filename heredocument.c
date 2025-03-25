/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:33:18 by shiori            #+#    #+#             */
/*   Updated: 2025/03/25 21:02:49 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	create_temp_file(void)
{
	int	tmp_fd;

	tmp_fd = open("/tmp/minishell_heredoc", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (tmp_fd);
}


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

static int	write_heredoc_content(int tmp_fd, t_rdrct *rdrct, t_list *env)
{
	char *line;
	char *delimiter;
	struct termios orig_term;
	
	// 端末設定を保存
	tcgetattr(STDIN_FILENO, &orig_term);
	
	// ヒアドキュメント用のシグナル設定
	// setup_heredoc_signals();
	
	delimiter = rdrct->file[0];
	
	while (1)
	{
		// プロンプト表示と入力受付
		line = readline("> ");
		
		// Ctrl+Cで中断された場合
		if (g_last_exit_status == 1)
		{
			if (line)
				free(line);
				
			// 端末設定を元に戻す
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
			
			// 対話モードに戻す
			setup_interactive_signals();
			
			exit(1);
		}
		
		// EOF (Ctrl+D) チェック（nullptrの場合のみ）
		if (!line)
		{
			// 端末設定を元に戻す
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
			
			// 対話モードに戻す
			setup_interactive_signals();
			
			return (0);
		}
		
		// デリミタと完全一致で終了 - 空行は無視しない
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// 内容を一時ファイルに書き込み (空行も書き込む)
		if (rdrct->is_quoted)
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		else
		{
			// 空行でも改行は書き込む
			if (ft_strlen(line) > 0)
				write_expand_env(tmp_fd, line, env);
			write(tmp_fd, "\n", 1);
		}
		
		free(line);
	}
	
	// 端末設定を元に戻す
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
	
	// 対話モードに戻す
	setup_interactive_signals();
	
	return (0);
}

static int	setup_heredoc_input(int tmp_fd)
{
	int fd;
	
	// 書き込み用ファイルを閉じる
	close(tmp_fd);
	
	// 読み取り用に再オープン
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
		return (perror_int("open", errno));
	
	// 標準入力にリダイレクト
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (perror_int("dup2", errno));
	}
	
	// 一時ファイルを削除（内容はまだ読み取り可能）
	unlink("/tmp/minishell_heredoc");
	close(fd);
	
	return (0);
}

int	handle_heredocument(t_rdrct *redirect, t_cmd *cmd, t_list *env)
{
	int tmp_fd;
	int result;
	int original_stdin;
	
	// 標準入力のバックアップが必要なら作成
	if (cmd->backup_stdin == -1)
	{
		cmd->backup_stdin = dup(STDIN_FILENO);
		if (cmd->backup_stdin == -1)
			return (perror_int("dup", errno));
	}
	
	// バックアップから作業用コピーを作成
	original_stdin = dup(cmd->backup_stdin);
	if (original_stdin == -1)
		return (perror_int("dup", errno));
	
	// 標準入力を元の状態に戻す
	if (dup2(original_stdin, STDIN_FILENO) == -1)
	{
		close(original_stdin);
		return (perror_int("dup2", errno));
	}
	
	// 一時ファイル作成
	tmp_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
	{
		close(original_stdin);
		return (perror_int("open", errno));
	}
	
	// ヒアドキュメント内容の取得
	result = write_heredoc_content(tmp_fd, redirect, env);
	
	// 作業用標準入力を閉じる
	close(original_stdin);
	
	// エラーの場合は処理を中断
	if (result == -1)
	{
		close(tmp_fd);
		unlink("/tmp/minishell_heredoc");
		
		// 中断時に標準入力を確実に復元
		dup2(cmd->backup_stdin, STDIN_FILENO);
		
		return (-1);
	}
	
	// 一時ファイルを標準入力に設定
	if (setup_heredoc_input(tmp_fd) == -1)
		return (-1);
	
	return (0);
}