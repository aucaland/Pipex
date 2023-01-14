NAME		= pipex
SRC_DIR		= srcs
OBJ_DIR		= objs
SRC_NAME	= test.c

INCS_PIPEX = pipex.h


OBJ_NAME		=	$(SRC_NAME:%.c=%.o)

SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))

####Compil####

CC 		= gcc -O3
CFLAGS 	= -Wall -Wextra -Werror

### DEBUG ####
D = 0
ifeq ($(D), 1)
	CC += -fsanitize=address -g3
	MODE = echo "\033[0;31m MODE DEBUG SANITIZE \033[0m"
endif
ifneq ($(D), 1)
	MODE = echo "\033[0;31m MODE RELEASE \033[0m"
endif

#####LIBS#####

CH_FLAG	= -I. -ILIBFT

## Rules ##

all:			$(NAME)

$(NAME):	$(OBJ)
	@echo "Making LIBFT"
	@make all -C LIBFT
	@$(CC) $^ -o $(NAME) $(CFLAGS) $(CH_FLAG) -L./LIBFT -lft
	@echo  "-\033[1;35mEdit/Create: \033[0m $?                    \033[0;32m[OK]\033[0m"
	@$(MODE)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(INCS_PIPEX)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CH_FLAG) -o $@ -c $<
	@echo "-\033[1;92mCompiling : \033[0m $?"

clean:
	@rm -rf $(OBJ)
	@make clean -C LIBFT
	@echo  "-\033[1;33m Remove objs file  \033[0m  \".o\"         \033[0;32m [OK] \033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C LIBFT
	@echo  "-\033[1;33m Remove \033[0m             \"$(NAME)\"    \033[0;32m [OK] \033[0m"

re: fclean all

.PHONY: all clean fclean re
