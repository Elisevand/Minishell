NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./includes -I./libft
LDFLAGS = -lreadline

SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = objs
LIBFT_DIR = libft

SRCS =	$(SRC_DIR)/minishell.c \
		$(SRC_DIR)/signal/signal.c \
		$(SRC_DIR)/tokenizer/tokenizer.c \
		$(SRC_DIR)/tokenizer/tokenizer_utils.c \
		$(SRC_DIR)/tokenizer/tokenizer_heredoc.c \
		$(SRC_DIR)/tokenizer/tokenize_input.c \
		$(SRC_DIR)/tokenizer/extract_word.c \
		$(SRC_DIR)/utils/utils.c \
		$(SRC_DIR)/process/process.c \
		$(SRC_DIR)/process/syntax_validation.c \
		$(SRC_DIR)/process/validation_utils.c \
		$(SRC_DIR)/expand/expand_variables.c	\
		$(SRC_DIR)/expand/expand_utils.c \
		$(SRC_DIR)/expand/expand_cmd.c \
		$(SRC_DIR)/execute/execute_core.c \
		$(SRC_DIR)/execute/execute_setup.c \
		$(SRC_DIR)/execute/execute_heredoc.c \
		$(SRC_DIR)/execute/execute_redirect.c \
		$(SRC_DIR)/execute/execute_process.c \
		$(SRC_DIR)/execute/execute_process_utils.c \
		$(SRC_DIR)/execute/execute_pipeline.c \
		$(SRC_DIR)/execute/execute_pipeline_utils.c \
		$(SRC_DIR)/execute/execute_pipeline_process.c \
		$(SRC_DIR)/execute/execute_pipeline_process2.c \
		$(SRC_DIR)/execute/execute_pipeline_wait.c \
		$(SRC_DIR)/execute/execute_path_utils.c \
		$(SRC_DIR)/execute/execute_pipeline_cmd.c \
		$(SRC_DIR)/execute/execute_command.c \
		$(SRC_DIR)/execute/execute_pipeline_cmd_utils.c \
		$(SRC_DIR)/execute/execute_pipeline_cmd_utils2.c \
		$(SRC_DIR)/execute/execute_error.c \
	   	$(SRC_DIR)/execute/execute_heredoc_signal.c \
	   	$(SRC_DIR)/execute/execute_pipeline_redirect.c \
		$(SRC_DIR)/builtin/builtin.c \
	   	$(SRC_DIR)/builtin/builtin_cd_oldpwd.c \
	   	$(SRC_DIR)/builtin/builtin_cd_path_utils.c \
	   	$(SRC_DIR)/builtin/builtin_cd_path.c \
	   	$(SRC_DIR)/builtin/builtin_cd_path2.c \
	   	$(SRC_DIR)/builtin/builtin_cd.c \
	   	$(SRC_DIR)/builtin/builtin_exit.c \
	   	$(SRC_DIR)/builtin/builtin_export.c \
	   	$(SRC_DIR)/builtin/builtin_export_utils.c \
		$(SRC_DIR)/env/env_create.c \
		$(SRC_DIR)/env/env_free.c \
		$(SRC_DIR)/env/env_init.c \
		$(SRC_DIR)/env/env_set.c \
		$(SRC_DIR)/env/env_utils.c \
		$(SRC_DIR)/env/env_utils2.c \
		$(SRC_DIR)/env/env_utils3.c \
		$(SRC_DIR)/parser/parser.c \
		$(SRC_DIR)/parser/parser_utils.c \
		$(SRC_DIR)/parser/parser_utils2.c \
		$(SRC_DIR)/parser/parser_free.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBFT = $(LIBFT_DIR)/libft.a

all: libft_make $(NAME)

libft_make:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

# Compilation des .o en créant les dossiers si besoin
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft_make
