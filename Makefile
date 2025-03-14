# Makefile for Minishell

NAME = minishell

CC = cc
RM = rm
FSANITIZE = -fsanitize=address
CFLAGS = -Wall -Wextra -Werror 
INCLUDES = -I./includes -I/usr/local/opt/readline/include
# READLINE = -I/usr/local/Cellar/readline/8.2.13/include -D_DARWIN_C_SOURCE -L/usr/local/Cellar/readline/8.2.13/lib -lreadline
READLINE = -lreadline -L/usr/local/opt/readline/lib

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

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
unset.c\
env_utils.c\
utils.c\

ENV_SRCS =\
env_convert.c\
env_delete.c\
env_free.c\
env_get.c\
env_update.c\
env_utils.c

OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

PURSER = purser
PURSER_TEST_MAIN = purser_test_main.c
PURSER_SRCS =\
	purser_args.c\
	purser_free.c\
	purser_quote_env.c\
	purser_redirect.c\
	purser.c\
	$(ENV_SRCS)
PURSER_OBJ_DIR = purser_obj
PURSER_OBJS = $(addprefix $(PURSER_OBJ_DIR)/, $(PURSER_SRCS:.c=.o))

.PHONY: all p clean fclean re

# デフォルトターゲット
all: $(NAME)

# バイナリ生成ルール
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(READLINE) $(OBJS) $(LIBFT) -o $(NAME)

# ソースファイルからオブジェクトファイル生成
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# オブジェクトファイル生成ディレクトリ生成
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

p: $(PURSER)

$(PURSER): $(PURSER_TEST_MAIN) $(PURSER_OBJS) $(LIBFT) | $(PURSER_OBJ_DIR)
	$(CC) $(CFLAGS) $(PURSER_TEST_MAIN) $(PURSER_OBJS) $(LIBFT) $(INCLUDES) -o $(PURSER)

$(PURSER_OBJ_DIR)/%.o: %.c | $(PURSER_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PURSER_OBJ_DIR):
	mkdir -p $(PURSER_OBJ_DIR)

# libftのビルドルール
$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

# クリーンアップ
clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) -frd $(OBJ_DIR) $(PURSER_OBJ_DIR)

# 全ての生成物を削除
fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) -f $(NAME) $(PURSER)

# 再ビルド
re: fclean all