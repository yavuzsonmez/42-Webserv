# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/21 13:57:17 by ysonmez           #+#    #+#              #
#    Updated: 2022/07/25 13:39:18 by ysonmez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

Reset			=		\033[0m						# Text Reset
R				=		\033[0;31m					# Red
G				=		\033[0;32m					# Green
B				=		\033[0;34m					# Blue
P				=		\033[0;35m					# Purple

NAME			=		webserv

HDRS			=		/inc/

SRCS			=		/src/

OBJS			=		$(SRCS:.cpp=.o)

DEBUG			=		-g -fsanitize=address

FLAGS			=		-Wall -Werror -Wextra -Wshadow -Wno-shadow -std=c++98

.c.o			:
						@c++ $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME)			:		$(OBJS) $(HDRS) | silence
						@c++ $(FLAGS) $(OBJS) -o $(NAME)
						@echo "$(G)$(NAME) has been created$(Reset)"

dev				:		$(OBJS) $(HDRS) | silence
						@c++ $(OBJS) -o $(NAME)
						@echo "$(R)Compiling without flags WARNING !$(Reset)"
						@echo "$(G)$(NAME) has been created$(Reset)"

debug				:	$(OBJS) $(HDRS) | silence
						@c++ $(OBJS) $(DEBUG) -o $(NAME)
						@echo "$(P)DEBUG MODE : address sanitizer$(Reset)"
						@echo "$(G)$(NAME) has been created$(Reset)"
						$(NAME) > debug.log
						@echo "$(B)Debug logged in 'debug.log'$(Reset)"

silence:
						@:

valgrind			:	$(NAME)
						@echo "$(B)Checking for memory leaks..$(Reset)"
						@valgrind --leak-check=full $(NAME)

clean			:
						@rm -f $(OBJS)
						@echo "$(R)Objects have been removed$(Reset)"

fclean			:		clean
						@rm -f $(NAME)
						@echo "$(R)$(NAME) has been removed$(Reset)"
						@rm -f debug.log
						@rm -f leaks.log
						@echo "$(R)logs have been removed$(Reset)"

all				:		$(NAME)
						$(NAME)

re				:		fclean all

.PHONY			:		clean fclean all re