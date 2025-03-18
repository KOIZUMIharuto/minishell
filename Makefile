# Makefile for Minishell

NAME = minishell

CC = cc
RM = rm
FSANITIZE = -fsanitize=address
CFLAGS = -Wall -Wextra -Werror 
INCLUDES = -I./includes -I/usr/local/opt/readline/include
READLINE = -lreadline -L/usr/local/opt/readline/lib

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# src
SRCS =\
error.c\
execve.c\
heredocument.c\
pipe.c\
redirect.c\
signal.c\
free.c \
stdio_fd_utils.c \

# builtin
BUILTIN_SRCS =\
builtin_cd.c\
builtin_echo.c\
builtin_env.c\
builtin_exit.c\
builtin_export.c\
builtin_pwd.c\
builtin_unset.c\
builtin.c

# env
ENV_SRCS =\
env_init.c\
env_convert_to_array.c \
env_delete.c\
env_free.c\
env_get.c\
env_update.c\
env_utils.c

PARSER_SRCS =\
parser_args.c\
parser_free.c\
parser_quote_env.c\
parser_redirect.c\
parser.c

SRCS += $(BUILTIN_SRCS)	$(ENV_SRCS) $(PARSER_SRCS)

OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

MAIN = $(OBJ_DIR)/main.o

# parser
PARSER = parser
PARSER_TEST_MAIN = $(OBJ_DIR)/parser_test_main.o

.PHONY: all p clean fclean re

# デフォルトターゲット
all: $(NAME)

# バイナリ生成ルール
$(NAME): $(MAIN) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(READLINE) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFT) -o $(NAME)

p: $(PARSER)

$(PARSER): $(PARSER_TEST_MAIN) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(READLINE) $(INCLUDES) $(PARSER_TEST_MAIN) $(OBJS) $(LIBFT)  -o $(PARSER)

# ソースファイルからオブジェクトファイル生成
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# オブジェクトファイル生成ディレクトリ生成
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# libftのビルドルール
$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

# クリーンアップ
clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) -frd $(OBJ_DIR)

# 全ての生成物を削除
fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) -f $(NAME) $(PARSER)

# 再ビルド
re: fclean all