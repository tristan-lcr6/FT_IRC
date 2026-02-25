# FT_IRC
NAME = ircserv
NAME_ARGS = <port> <password>
BONUS_ARGS = 

# Source files
SRC_DIR = src/
SRC_FILES = main.cpp Server.cpp Client.cpp
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_BONUS_FILES = 
SRC_BONUS = $(addprefix $(SRC_DIR), $(SRC_BONUS_FILES))

# Object files
OBJ_DIR = objs/
OBJ := $(addprefix $(OBJ_DIR), $(SRC_FILES:%.cpp=%.o))
OBJ_BONUS := $(addprefix $(OBJ_DIR), $(SRC_BONUS_FILES:%.cpp=%.o))

# Includes and libraries
#	Header files
INC_DIR = includes/

# Compiling setup
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS = -I$(INC_DIR)

# Font styles (ANSI escape codes)
DEFAULT = \033[0m
WHITE = \033[37m
RED = \033[91m
GREEN = \033[92m
YELLOW = \033[93m
CYAN = \033[96m
UNDERLINE = \033[4m
DOUBLE_UL = \033[21m
UNDERLINE_E = \033[24m
BOLD = \033[1m
BOLD_E = \033[22m
ITALIC = \033[3m
ITALIC_E = \033[23m
BLINK = \033[5m
BLINK_E = \033[25m
INVERTED = \033[7m
INVERTED_E = \033[27m

all:
	@len=$$(echo "$(NAME)" | wc -m); pad=$$((len+13)); \
	echo "$(CYAN)$(BOLD)╔$$(printf '═%.0s' $$(seq 1 $$pad))╗"; \
	echo "║ Welcome to $(NAME)! ║"; \
	echo "╚$$(printf '═%.0s' $$(seq 1 $$pad))╝$(DEFAULT)"
	@echo "$(CYAN)$(BOLD)We are about to compile everything for you, this may take a while.$(DEFAULT)"
	@$(MAKE) --no-print-directory $(NAME)

$(NAME): $(OBJ)
	@echo "$(GREEN)All sources compiled!$(DEFAULT)"
	@echo "$(YELLOW)Linking $@...$(DEFAULT)"
	@$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "$(GREEN)SUCCESS! You can now use $(BOLD)$(ITALIC)'./$@ $(NAME_ARGS)'$(DEFAULT)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(DEFAULT)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

bonus: clean $(OBJ_BONUS)
	@echo "$(GREEN)All sources compiled for the bonus!$(DEFAULT)"
	@echo "$(YELLOW)Linking $(NAME)...$(DEFAULT)"
	@$(CC) $(CFLAGS) $(OBJ_BONUS) -o $(NAME)
	@echo "$(GREEN)SUCCESS! You can now use $(BOLD)$(ITALIC)'./$(NAME) $(BONUS_ARGS)'$(DEFAULT)"

clean:
	@echo "$(YELLOW)Erasing objects...$(DEFAULT)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Cleaned!$(DEFAULT)"

fclean: clean
	@echo "$(YELLOW)Erasing $(NAME)...$(DEFAULT)"
	@rm -rf $(NAME)
	@echo "$(GREEN)Everything fully cleaned!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re bonus
