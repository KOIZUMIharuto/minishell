# Makefile for Minishell

NAME = minishell
CC = cc
FSANITIZE = -fsanitize=address
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes
LIBFT = libft/libft.a
SRCS = main.c echo.c exit.c pwd.c cd.c export.c unset.c env.c execve.c signal.c redirect.c execute_bultin.c pipe.c \
       heredocument.c
OBJS = $(SRCS:.c=.o)
READLINE = -lreadline
RM = rm -f  # 削除コマンドを変数に定義

PURSER = purser
PURSER_SRCS =\
	purser_args.c\
	purser_free.c\
	purser_quote_env.c\
	purser_redirect.c\
	purser.c
PURSER_OBJ_DIR = purser_obj
PURSER_OBJS = $(addprefix $(PURSER_OBJ_DIR)/, $(PURSER_SRCS:.c=.o))

.PHONY: all clean fclean re

# デフォルトターゲット
all: $(NAME)

# バイナリ生成ルール
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

# libftのビルドルール
$(LIBFT):
	$(MAKE) -C libft bonus

# ソースファイルからオブジェクトファイル生成
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# クリーンアップ
clean:
	$(MAKE) clean -C libft
	$(RM) $(OBJS)
	$(RM) -frd $(PURSER_OBJ_DIR)

# 全ての生成物を削除
fclean: clean
	$(MAKE) fclean -C libft
	$(RM) $(NAME) $(PURSER)

p: $(PURSER)

$(PURSER): $(PURSER_OBJS) $(LIBFT) | $(PURSER_OBJ_DIR)
	$(CC) $(CFLAGS) $(PURSER_OBJS) $(LIBFT) $(INCLUDES) -o $(PURSER)

$(PURSER_OBJ_DIR)/%.o: %.c | $(PURSER_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PURSER_OBJ_DIR):
	mkdir -p $(PURSER_OBJ_DIR)

# 再ビルド
re: fclean all