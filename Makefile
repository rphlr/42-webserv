# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/26 14:19:55 by rrouille          #+#    #+#              #
#    Updated: 2024/03/01 17:07:41 by ckarl            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98 -fsanitize=address
NAME		= webserv
OBJS_DIR	= objs
SRCS_DIR	= srcs

# Source files
MAIN_SRC		= main.cpp
CONFIG_SRC		= Parser.cpp Server.cpp Location.cpp

# Object files
MAIN_OBJ		= $(addprefix $(OBJS_DIR)/, $(MAIN_SRC:.cpp=.o))
CONFIG_OBJ		= $(addprefix $(OBJS_DIR)/config/, $(CONFIG_SRC:.cpp=.o))

# List of all object files
OBJS			= $(MAIN_OBJ) $(CONFIG_OBJ)

### COLORS ###
RED         = \033[1;31m
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
BLUE        = \033[1;34m
VIOLET      = \033[1;35m
CYAN        = \033[1;36m
WHITE       = \033[1;37m
RESET       = \033[0m

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	/bin/rm -rf $(OBJS_DIR)

fclean: clean
	/bin/rm -f $(NAME)
	@echo "$(BLUE)All clean! ✨$(RESET)"

re: fclean all
	@echo "$(GREEN)Cleaned & rebuilt! 💫$(RESET)"

.PHONY: all clean fclean re