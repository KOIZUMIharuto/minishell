# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "includes/minishel.h"


int main() {
    char *line;
    char **tokens;

    while (1) {
        // ユーザー入力を取得
        line = readline("> ");
        if (line == NULL) {  // EOF (Ctrl+D)
            printf("\n");/////
            break;
        }

        if (strlen(line) > 0) {
            add_history(line);  // 履歴に追加
        }

        // 入力をスペースで分割
        tokens = ft_split(line, ' ');

        if (tokens[0] != NULL) {
            printf("Command: %s\n", tokens[0]);

            printf("Args: ");
            for (int i = 1; tokens[i] != NULL; ++i) {
                printf("%s ", tokens[i]);
            }
            printf("\n");
        }

        // メモリ解放
        free(line);
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}