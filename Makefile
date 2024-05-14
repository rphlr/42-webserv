# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/29 12:40:01 by rrouille          #+#    #+#              #
#    Updated: 2024/05/14 20:12:25 by rrouille         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                             Makefile Configuration                           #
#                                                                              #
# **************************************************************************** #

# Name of the executable
NAME				= webserv
PROGRAMMERS			= rrouille, nvaubien, ckarl

# Arguments passed to the executable
ARGS				= $(filter-out $@,$(MAKECMDGOALS))

# Directories for source and object files
SRCSDIR				= srcs
OBJSDIR				= objs
HDRDIR				= includes

# Terminal color settings for output messages
GRAY				= \033[0;90m
RED					= \033[0;91m
ORANGE				= \033[0;33m
GREEN				= \033[0;92m
YELLOW				= \033[0;93m
BLUE				= \033[0;94m
MAGENTA				= \033[0;95m
CYAN				= \033[0;96m
WHITE				= \033[0;97m
ENDCOLOR			= \033[0m

# Background colors for terminal
BG_G				= \033[42m
BG_R				= \033[41m
BG_Y				= \033[43m
BG_B				= \033[44m
BG_M				= \033[45m
BG_C				= \033[46m
BG_W				= \033[47m

# Colors and ANSI escape codes for cursor movement
CLEAR_LINE			= \033[2K
MOVE_BEGIN			= \033[1G
MOVE_UP				= \033[1A
BOLD				= \033[1m
UNDERLINE			= \033[4m
ITALIC				= \033[3m
INVERTED			= \033[7m
BOX_WIDTH			= 80

# Command to clear the screen
CLEAR				= \033c

# Source and object files
SRCS				= $(shell find ${SRCSDIR} -type f -name '*.cpp')
OBJS				= $(SRCS:${SRCSDIR}/%.cpp=${OBJSDIR}/%.opp)
CFLAGS				= -Wall -Wextra -Werror -std=c++98
# LDFLAGS			= -g3 -fsanitize=address
CC					= c++
RM					= rm -rf
MAKE				= make
MKDIR				= mkdir -p

UNAME 				= $(shell uname)
ARCH				= $(shell uname -m)

DFLAGS				= DEBUG=1
COMPILATION_DONE	= 0

# Create a variable to check if the .WAIT target is supported
WAIT_SUPPORTED := $(shell echo "all: .WAIT" > Makefile.test && ( make -f Makefile.test all 2>/dev/null && echo YES || echo NO) && rm -f Makefile.test)

ifeq ($(WAIT_SUPPORTED),YES)
    WAIT_CMD := .WAIT
else
    WAIT_CMD :=
endif

# **************************************************************************** #
#                           Compilation Rules                                  #
# **************************************************************************** #

# Rule to compile the entire program
all:			prog_info ${WAIT_CMD} ${NAME}
				${COMPILATION}
				@if [ -d "${OBJSDIR}" ] && [ "$$(ls ${OBJSDIR}/*.opp 2>/dev/null)" ]; then \
					newest_obj=$$(ls -t ${OBJSDIR}/*.opp ${OBJSDIR}/*/*.opp | head -1); \
					if [ -f "${NAME}" ] && [ "${NAME}" -nt "$$newest_obj" ]; then \
						printf "${GREEN}┌${BOLD}COMPILATION INFO${ENDCOLOR}${GREEN}──────────────────────────────────────────────────────────┐\n"; \
						printf "│${ENDCOLOR} Nothing to be done for ${BOLD}${NAME}${ENDCOLOR}.                                          ${GREEN}│\n"; \
						printf "└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"; \
					else \
						printf "\033[A\033[2K"; \
						printf "${GREEN}┌${BOLD}COMPILATION INFO${ENDCOLOR}${GREEN}──────────────────────────────────────────────────────────┐\n"; \
						printf "│${ENDCOLOR} ${BOLD}${NAME}${ENDCOLOR} compiled! Launch it with \'${BOLD}./${NAME}${ENDCOLOR}\'.                            ${GREEN}│\n"; \
						printf "└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"; \
					fi \
				else \
					printf "${GREEN}┌${BOLD}COMPILATION INFO${ENDCOLOR}${GREEN}─────────────────────────────────────────────────────────┐\n"; \
					printf "│${ENDCOLOR} Nothing to be done for ${BOLD}${NAME}${ENDCOLOR}.                                         ${GREEN}│\n"; \
					printf "└─────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"; \
				fi

# Rule to compile the main executable
${NAME}:		${OBJS}
				${COMPILATION}

# Rule to compile object files
${OBJSDIR}/%.opp: ${SRCSDIR}/%.cpp
				@${MKDIR} ${@D}
				@$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
				@if [ $(COUNT) -eq 1 ]; then \
					printf "${YELLOW}┌${BOLD}COMPILATION${ENDCOLOR}${YELLOW}───────────────────────────────────────────────────────────────┐\n"; \
					printf "${YELLOW}│${ENDCOLOR} ${BOLD}${COUNT}${ENDCOLOR} out of ${BOLD}${words $(SRCS)}${ENDCOLOR} files compiled.                                              ${YELLOW}│${ENDCOLOR}\n"; \
					printf "${YELLOW}└──────────────────────────────────────────────────────────────────────────┘\n"; \
					printf "${MOVE_UP}${MOVE_UP}"; \
				else \
					printf "${CLEAR_LINE}"; \
					if [ $(COUNT) -lt $(words $(SRCS)) ]; then \
						printf "${YELLOW}│${ENDCOLOR} ${BOLD}${COUNT}${ENDCOLOR} out of ${BOLD}${words $(SRCS)}${ENDCOLOR} files compiled.                                              ${YELLOW}│${ENDCOLOR}${MOVE_BEGIN}"; \
					else \
						printf "${YELLOW}│${ENDCOLOR} All sources compiled for ${BOLD}${NAME}${ENDCOLOR}.                                        ${YELLOW}│${ENDCOLOR}\n\n\n"; \
					fi; \
				fi
				@${CC} ${CFLAGS} -c $< -o $@ -I${HDRDIR}

CC_PROG			= ${CC} ${CFLAGS} ${OBJS} -o ${NAME}
COMPILATION		= ${CC_PROG}

run:
				@if [ ! -f "${NAME}" ]; then \
					${MAKE} all; \
				fi
				@printf "${GREEN}┌${BOLD}RUNNING${ENDCOLOR}${GREEN}───────────────────────────────────────────────────────────────────┐\n"
				@printf "${GREEN}│${ENDCOLOR} Running ${BOLD}${NAME}${ENDCOLOR}...                                                       ${GREEN}│\n"
				@printf "${GREEN}└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"
				@./${NAME} ${ARGS}

debug:
				@if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "0" ]]; then \
					printf "┌──────────\n"; \
				fi
				@printf "├──────────\n"
				@${MAKE} ${OBJS} dbg
				@printf "│\t${YELLOW}DEBUG${ENDCOLOR} Sources for ${GREEN}${NAME}${ENDCOLOR} done.\n"
				@${CC} ${OBJS} -o ${NAME} ${LFT} ${LFLAGS} ${LDFLAGS}
				@printf "├──────────\n├─>>> ${YELLOW}DEBUG${ENDCOLOR} ${GREEN}${NAME}${ENDCOLOR} compiled!\n└──────────\n"

# Rebuild the entire project
re:				fclean ${WAIT_CMD} all

################################################################################
#                              Usefuls                                         #
################################################################################

# Rule to display the project information
prog_info:
				@printf "${BLUE}┌${BOLD}PROJECT INFO${ENDCOLOR}${BLUE}──────────────────────────────────────────────────────────────┐\n"
				@printf "${BLUE}│${ENDCOLOR} Project name:\t\t${BOLD}${NAME}${ENDCOLOR}                                            ${BLUE}│\n"
				@printf "${BLUE}│${ENDCOLOR} Source folder:\t${BOLD}${SRCSDIR}${ENDCOLOR}/                                              ${BLUE}│\n"
				@printf "${BLUE}│${ENDCOLOR} Object folder:\t${BOLD}${OBJSDIR}${ENDCOLOR}/                                              ${BLUE}│\n"
				@printf "${BLUE}│${ENDCOLOR} Header folder:\t${BOLD}${HDRDIR}${ENDCOLOR}/                                          ${BLUE}│\n"
				@printf "${BLUE}├──────────────────────────────────────────────────────────────────────────┤\n"
				@printf "${BLUE}│${ENDCOLOR} Made by\t\t${BOLD}${PROGRAMMERS}${ENDCOLOR}                          ${BLUE}│\n"
				@printf "${BLUE}└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"

################################################################################
#                       Cleaning and Maintenance                               #
################################################################################

# Clean object files and executable
clean:
				@if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
					printf "${ORANGE}├──────────────────────────────────────────────────────────────────────────┤${ENDCOLOR}\n"; \
				else \
					printf "${ORANGE}┌${BOLD}CLEANING${ENDCOLOR}${ORANGE}──────────────────────────────────────────────────────────────────┐\n"; \
				fi
				@if [ -d "./${OBJSDIR}" ]; then \
					printf "${ORANGE}│${ENDCOLOR} Removing ${BOLD}${OBJSDIR}${ENDCOLOR} for ${BOLD}${NAME}${ENDCOLOR}.                                               ${ORANGE}│${ENDCOLOR}\n"; \
					rm -rf ${OBJSDIR}; \
				else \
					printf "${ORANGE}│${ENDCOLOR} ${BOLD}${OBJSDIR}${ENDCOLOR} already removed for ${BOLD}${NAME}${ENDCOLOR}!                                        ${ORANGE}│${ENDCOLOR}\n"; \
				fi
				if [ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]; then \
					printf "${ORANGE}├──────────────────────────────────────────────────────────────────────────┤${ENDCOLOR}\n"; \
				else \
					printf "${ORANGE}└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"; \
				fi

# Clean everything including the executable
fclean: 
				@printf "${RED}┌${BOLD}DEEP CLEANING${ENDCOLOR}${RED}─────────────────────────────────────────────────────────────┐\n"
				@if [ -d "./${OBJSDIR}" ]; then \
					printf "${RED}│${ENDCOLOR} Removing ${BOLD}${OBJSDIR}${ENDCOLOR} for ${BOLD}${NAME}${ENDCOLOR}.                                               ${RED}│${ENDCOLOR}\n"; \
					rm -rf ${OBJSDIR}; \
				else \
					printf "${RED}│${ENDCOLOR} ${BOLD}${OBJSDIR}${ENDCOLOR} already removed for ${BOLD}${NAME}${ENDCOLOR}!                                        ${RED}│${ENDCOLOR}\n"; \
				fi
				@printf "${RED}├──────────────────────────────────────────────────────────────────────────┤${ENDCOLOR}\n"
				@if [ -e "./${NAME}" ]; then \
					printf "${RED}│${ENDCOLOR} Removing ${BOLD}${NAME}${ENDCOLOR}.                                                        ${RED}│${ENDCOLOR}\n"; \
					rm -rf ${NAME}; \
				else \
					printf "${RED}│${ENDCOLOR} ${BOLD}${NAME}${ENDCOLOR} already removed!                                                 ${RED}│${ENDCOLOR}\n"; \
				fi
				@printf "${RED}└──────────────────────────────────────────────────────────────────────────┘${ENDCOLOR}\n"

################################################################################
#                                 Github                                       #
################################################################################

# Rule to pull changes from Git
pull:
				@git pull

export MAKEFLAGS += --silent

.PHONY: all fclean clean re