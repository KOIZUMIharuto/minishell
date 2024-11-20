# Makefile for Minishell

NAME = minishell

CC = cc
RM = rm

CFLAGS = -Wall -Wextra -Werror
READLINE = -lreadline
INCLUDES = -I ./includes

LIBFT_DIR = libft
LIBFT_NAME = libft.a

SRCS = \
cd.c\
echo.c\
env.c\
execute_bultin.c\
execve.c\
exit.c\
export.c\
heredocument.c\
main.c\
pipe.c\
pwd.c\
redirect.c\
signal.c\
unset.c
OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.PHONY: all clean fclean re

# デフォルトターゲット
all: $(NAME)

# バイナリ生成ルール
$(NAME): $(OBJS) $(LIBFT_DIR)/$(LIBFT_NAME)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/$(LIBFT_NAME) $(READLINE) -o $(NAME)

# libftのビルドルール
$(LIBFT_DIR)/$(LIBFT_NAME):
	$(MAKE) -C $(LIBFT_DIR)

# ソースファイルからオブジェクトファイル生成
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# オブジェクトファイル生成ディレクトリ生成
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# クリーンアップ
clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) -fr $(OBJ_DIR)

# 全ての生成物を削除
fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) -f $(NAME)

# 再ビルド
re: fclean all