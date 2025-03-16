# include <minishell.h>
# include "../libft/get_next_line.h"

int handle_heredocument(char *delimiter, t_cmd *cmd) {
    size_t delimiter_len = ft_strlen(delimiter);
    int pipe_fds[2];
    
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        return -1;
    }

    cmd->backup_stdin = dup(STDIN_FILENO);
    if (cmd->backup_stdin == -1)
    {
        perror("dup");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(cmd->backup_stdin);
        cmd->backup_stdin = -1;
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return -1;
    }

    if (pid == 0) { // 子プロセス
        close(pipe_fds[0]);
        
        char *line;                
        while (1) {
            write(STDOUT_FILENO, "> ", 2);
            line = get_next_line(STDIN_FILENO);
            // デリミタと一致したら、蓄積した内容を書き込んで終了
            if (!line || (ft_strncmp(line, delimiter, delimiter_len) == 0
				            && ft_strlen(line) - 1 == delimiter_len))
            {
                free(line);
                break;
            }
            write(pipe_fds[1], line, ft_strlen(line));
		            free(line);
        }
        close(pipe_fds[1]);
        exit(0);
    }

    // 親プロセス
    waitpid(pid, NULL, 0);
    close(pipe_fds[1]);

    if (dup2(pipe_fds[0], STDIN_FILENO) == -1) {
        perror("dup2");
        close(cmd->backup_stdin);
        cmd->backup_stdin = -1;
        close(pipe_fds[0]);
        return -1;
    }
    close(pipe_fds[0]);

    return 0;
}



