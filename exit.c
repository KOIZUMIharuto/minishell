#include "minishell.h"

void builtin_exit(char **args) {
    (void)args;
    printf("Exiting shell...\n");
    exit(0);
}
