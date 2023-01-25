NAME				= pipex
SRC_DIR				= srcs
OBJ_DIR				= objs
SRC_DIR_B			= srcs_b
OBJ_DIR_B			= objs_b
H_DIR				= h_files
H_DIR_B				= h_files_bonus
SRC_NAME			= pipex.c errors.c childs.c parsing.c pipex_utils.c pipex_utils.c
SRC_NAME_BONUS		= pipex_bonus.c errors_bonus.c childs_bonus.c parsing_bonus.c pipex_utils_bonus.c
INCS_NAME			= pipex.h errors.h
INCS_NAME_BONUS		= pipex_bonus.h errors_bonus.h


OBJ_NAME		=	$(SRC_NAME:%.c=%.o)
OBJ_NAME_B		=	$(SRC_NAME_BONUS:%.c=%.o)

SRC 				= $(addprefix $(SRC_DIR)/,$(SRC_NAME))
OBJ 				= $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))
SRC_B 				= $(addprefix $(SRC_DIR_B)/,$(SRC_NAME_BONUS))
OBJ_B 				= $(addprefix $(OBJ_DIR_B)/,$(OBJ_NAME_B))
INCS_PIPEX			= $(addprefix $(H_DIR)/,$(INCS_NAME))
INCS_PIPEX_BONUS	= $(addprefix $(H_DIR_B)/,$(INCS_NAME_BONUS))

####Compil####

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror

#####LIBS#####

CH_FLAG			= -Ih_files -ILIBFT
CH_FLAG_BONUS	= -Ih_files_bonus -ILIBFT

### DEBUG ####
D = 0

ifeq ($(D), 1)
	CC += -fsanitize=address -g3
	MODE = echo "\033[0;31m MODE DEBUG SANITIZE \033[0m"
endif
ifneq ($(D), 1)
	MODE = echo "\033[0;31m MODE RELEASE \033[0m"
endif

## Rules ##

all:		$(NAME)

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

$(OBJ_DIR_B)/%.o:	$(SRC_DIR_B)/%.c $(INCS_PIPEX_BONUS)
	@mkdir -p $(OBJ_DIR_B)
	@$(CC) $(CFLAGS) $(CH_FLAG_BONUS) -o $@ -c $<
	@echo "-\033[1;92mCompiling : \033[0m $?"

bonus: $(OBJ_B)
	@echo "Making LIBFT"
	@make all -C LIBFT
	@$(CC) $^ -o $(NAME) $(CFLAGS) $(CH_FLAG_BONUS) -L./LIBFT -lft
	@echo  "-\033[1;35mEdit/Create: \033[0m $?                    \033[0;32m[OK]\033[0m"
	@$(MODE)

clean:
	@rm -rf $(OBJ) $(OBJ_B)
	@make clean -C LIBFT
	@echo  "-\033[1;33m Remove objs file  \033[0m  \".o\"         \033[0;32m [OK] \033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C LIBFT
	@echo  "-\033[1;33m Remove \033[0m             \"$(NAME)\"    \033[0;32m [OK] \033[0m"

re: fclean all

.PHONY: all clean fclean re bonus
