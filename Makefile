# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/29 12:40:01 by rrouille          #+#    #+#              #
#    Updated: 2024/03/31 16:55:22 by rrouille         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                             Makefile Configuration                           #
#                                                                              #
# **************************************************************************** #

# Name of the executable
NAME				= Webserv

# Arguments passed to the executable
ARGS				= $(filter-out $@,${MAKECMDGOALS})

# Directories for source and object files
SRCSDIR				= srcs
OBJSDIR				= objs
HDRDIR				= includes

# Terminal color settings for output messages
GRAY				= \033[0;90m
RED					= \033[0;91m
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

# Text formatting options for output messages
BOLD				= \033[1m
UNDERLINE			= \033[4m
ITALIC				= \033[3m
INVERTED			= \033[7m

# Command to clear the screen
CLEAR				= \033c

# Source and object files
SRCS				= $(shell find ${SRCSDIR} -type f -name '*.cpp')
OBJS				= $(SRCS:${SRCSDIR}/%.cpp=${OBJSDIR}/%.opp)
# CFLAGS				= -Wall -Wextra -Werror -std=c++98
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
all:			${NAME}

# Rule to compile the main executable
${NAME}:		prog_name ${WAIT_CMD} ${OBJS}
				${COMPILATION}
				@if [ -d "${OBJSDIR}" ] && [ "$$(ls ${OBJSDIR}/*.opp 2>/dev/null)" ]; then \
					newest_obj=$$(ls -t ${OBJSDIR}/*.opp ${OBJSDIR}/*/*.opp ${OBJSDIR}/*/*/*.opp | head -1); \
					if [ -f "${NAME}" ] && [ "${NAME}" -nt "$$newest_obj" ]; then \
						printf "\033[A\033[2K"; \
						printf "┌──────────\n"; \
						printf "│\tNothing to be done for ${GREEN}${NAME}${ENDCOLOR}.\n└──────────\n"; \
					else \
						printf "\033[A\033[2K"; \
						printf "┌──────────\n"; \
						printf "│\tSources for ${GREEN}${NAME}${ENDCOLOR} done.\n"; \
						printf "├──────────\n├─>>> ${GREEN}${NAME}${ENDCOLOR} compiled!\n└──────────\n"; \
					fi \
				else \
					printf "\033[A\033[2K"; \
					printf "┌──────────\n"; \
					printf "│\tNothing to be done for ${GREEN}${NAME}${ENDCOLOR}.\n└──────────\n"; \
				fi

# Rule to compile object files
${OBJSDIR}/%.opp:	${SRCSDIR}/%.cpp
				@${MKDIR} ${@D}
				@if [[ $(if $(filter dbg,${MAKECMDGOALS}),1,0) == "1" ]]; then \
					printf "│\t > ${YELLOW}DEBUG${ENDCOLOR} Compiling ${YELLOW}$<${ENDCOLOR} with ${YELLOW}${DFLAGS}${ENDCOLOR} and ${YELLOW}${LDFLAGS}${ENDCOLOR}...\r"; \
					${CC} -D ${DFLAGS} -c $< -o $@ ${LDFLAGS}; \
				else \
					printf "│\t > Compiling ${YELLOW}$<${ENDCOLOR} for ${GREEN}${NAME}${ENDCOLOR}...\r"; \
					${CC} ${CFLAGS} -c $< -o $@ -I${HDRDIR}; \
				fi
				@printf "\33[2K"

CC_PROG			= ${CC} ${CFLAGS} ${OBJS} -o ${NAME}
COMPILATION		= ${CC_PROG}

run:
				@printf "┌──────────\n"
				@printf "│\tRunning ${GREEN}${NAME}${ENDCOLOR}...\n"
				@printf "└──────────\n"
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
				# @printf "\n┌──────────\n│ Cleaning and ${GREEN}recompiling${ENDCOLOR}...\n"
				# @${MAKE} fclean r
				# @${MAKE} all r

# **************************************************************************** #
#                                 UTILITIES                                    #
# **************************************************************************** #

prog_name:
				@printf "${RED}"
				@cat makefile_utils/prog_name
				@printf "${ENDCOLOR}\n"

help:
				@if [ "${PRINT_SCREEN}" = "YES" ]; then \
					${ECHO} "${CLEAR}\c"; \
					${MAKE} draw_help; \
					for i in 3 2 1 0; do \
						printf '\r${BLUE}Help will be shown in: %d${ENDCOLOR}' "$$i"; \
						sleep 1; \
						printf '${CLEARLN}'; \
					done; \
				fi
				@${ECHO} "${GRAY}🏃 Run ${ITALIC}\`./${NAME}\`${ENDCOLOR}${GRAY} to see the program in action.${ENDCOLOR}${GRAY}"
				@${ECHO} "${BOLD}${UNDERLINE}💡 TIPS: 💡${ENDCOLOR}${GRAY}"
				@${ECHO} "\t- You can also use ${ITALIC}\`make run\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make r\`${ENDCOLOR}${GRAY} to try it out."
				@${ECHO} "\t- Check for memory leaks with ${ITALIC}\`make leaks\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make l\`${ENDCOLOR}${GRAY}."
				@${ECHO} "\t- Check the 42 norm with ${ITALIC}\`make norm\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make n\`${ENDCOLOR}${GRAY}."
				@${ECHO} "\t- Check the forbidden functions with ${ITALIC}\`make check_forbidden\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make cf\`${ENDCOLOR}${GRAY}."
				@${ECHO} "\t- Use ${ITALIC}\`make lldb\`${ENDCOLOR}${GRAY} to run the program with lldb."
				@${ECHO} "\t- Use ${ITALIC}\`make fast\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make cf\`${ENDCOLOR}${GRAY} to compile the program faster."
				@${ECHO} "\t- Use ${ITALIC}\`make clear\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make c\`${ENDCOLOR}${GRAY} to clear the screen."
				@${ECHO} "\t- Use ${ITALIC}\`make clean\`${ENDCOLOR}${GRAY} to clean the object files."
				@${ECHO} "\t- Use ${ITALIC}\`make fclean\`${ENDCOLOR}${GRAY} to clean the object files and the executable."
				@${ECHO} ""
				@${ECHO} "${YELLOW}🌟 Use ${ITALIC}\`make help\`${ENDCOLOR}${YELLOW} or ${ITALIC}\`make h\`${ENDCOLOR}${YELLOW} to display these helpful tips. 🚀${ENDCOLOR}"
h:				help

################################################################################
#                       Cleaning and Maintenance                               #
################################################################################

# Clean object files and executable
clean:
				@if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
					printf "├──────────\n"; \
				else \
					printf "┌──────────\n"; \
				fi
				@if [ -d "./${OBJSDIR}" ]; then \
					printf "│\tRemoving ${RED}${OBJSDIR}${ENDCOLOR} for ${YELLOW}${NAME}${ENDCOLOR}\n"; \
					rm -rf ${OBJSDIR}; \
				else \
					printf "│\t${RED}${OBJSDIR}${ENDCOLOR} already removed for ${YELLOW}${NAME}${ENDCOLOR}!\n"; \
				fi
				if [ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]; then \
					printf "├──────────\n"; \
				else \
					printf "└──────────\n"; \
				fi

# Clean everything including the executable
fclean: 
				@printf "┌──────────\n"
				@if [ -d "./${OBJSDIR}" ]; then \
					printf "│\tRemoving ${RED}${OBJSDIR}${ENDCOLOR} for ${YELLOW}${NAME}${ENDCOLOR}\n"; \
					rm -rf ${OBJSDIR}; \
				else \
					printf "│\t${RED}${OBJSDIR}${ENDCOLOR} already removed for ${YELLOW}${NAME}${ENDCOLOR}!\n"; \
				fi
				@printf "\033[A\033[2K"
				@if [ -e "./${NAME}" ]; then \
					printf "│\tRemoving ${YELLOW}${NAME}${ENDCOLOR}\n"; \
					rm -rf ${NAME}; \
				else \
					printf "│\t${YELLOW}${NAME}${ENDCOLOR} already removed!\n"; \
				fi
				@printf "└──────────\n"

################################################################################
#                                 Github                                       #
################################################################################

# Rule to push changes to Git
push:			fclean
				@git add .
				@if [ -z "${ARGS}" ]; then \
					echo "${RED}❌ Please provide a commit message! ✨${ENDCOLOR}"; \
					exit 1; \
				fi
				@git commit -m "${ARGS}"
				@git push

# Rule to pull changes from Git
pull:
				@git pull

export MAKEFLAGS += --silent

.PHONY: all fclean clean re