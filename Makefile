# Makefile for Minishell

NAME = minishell

CC = cc
RM = rm
FSANITIZE = -fsanitize=address
CFLAGS = -Wall -Wextra -Werror 

# OSに応じた設定
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # MacOS用の設定
    INCLUDES = -I./includes -I/usr/local/opt/readline/include
    READLINE = -L/usr/local/opt/readline/lib -lreadline
else
    # Linux用の設定
    INCLUDES = -I./includes
    READLINE = -lreadline
endif

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# src
SRCS =\
error.c\
execve.c\
free.c \
print.c\
signal.c\

# pipe
PIPE_SRCS =\
pipe.c \
pipe_command.c \
pipe_utils.c \

# redirect and heredocument
REDIRECT_HEREDOC_SRCS =\
redirect.c \
heredocument.c\

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
parser_expand_env_quote_utils.c\
parser_expand_env_quote.c\
parser_free.c\
parser_redirect_utils.c\
parser_redirect.c\
parser_split_tokens.c\
parser_syntax.c\
parser_tokenize.c\
parser_tokens_to_cmd.c\
parser_utils.c\
parser.c

SRCS += $(PIPE_SRCS) $(REDIRECT_HEREDOC_SRCS) $(BUILTIN_SRCS) $(ENV_SRCS) $(PARSER_SRCS)

OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

MAIN = $(OBJ_DIR)/main.o

.PHONY: all bonus clean fclean re

# デフォルトターゲット
all: $(NAME)

bonus: $(NAME)

# バイナリ生成ルール（リンカオプションの順序を修正）
$(NAME): $(MAIN) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

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
	$(RM) -f $(NAME)

# 再ビルド
re: fclean all