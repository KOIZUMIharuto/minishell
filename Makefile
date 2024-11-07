# Makefile for Minishell

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes -I./libft
LIBFT = libft/libft.a
SRCS = main.c echo.c exit.c pwd.c cd.c export.c unset.c env.c
OBJS = $(SRCS:.c=.o)
READLINE = -lreadline
RM = rm -f  # 削除コマンドを変数に定義

.PHONY: all clean fclean re

# デフォルトターゲット
all: $(NAME)

# バイナリ生成ルール
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

# libftのビルドルール
$(LIBFT):
	$(MAKE) -C libft

# ソースファイルからオブジェクトファイル生成
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# クリーンアップ
clean:
	$(MAKE) clean -C libft
	$(RM) $(OBJS)

# 全ての生成物を削除
fclean: clean
	$(MAKE) fclean -C libft
	$(RM) $(NAME)

# 再ビルド
re: fclean all