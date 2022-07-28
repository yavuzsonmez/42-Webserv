# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/21 13:57:17 by ysonmez           #+#    #+#              #
#    Updated: 2022/07/26 17:21:02 by ysonmez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

Reset			=		\033[0m						# Text Reset
R				=		\033[0;31m					# Red
G				=		\033[0;32m					# Green
B				=		\033[0;34m					# Blue
P				=		\033[0;35m					# Purple
Y				=		\033[33;1m					# Yellow

NAME			=		webserv

HDRS			=		./inc/utility.hpp \
						./inc/request.hpp \

PARSING			=		./src/parsing/request.cpp \

UTILS			=		./src/utility/get_file_content.cpp \
						./src/utility/is_file_accessible.cpp \
						./src/utility/lower_upper_str.cpp \
						./src/utility/check_config_file.cpp \
						./src/utility/strip_from_str.cpp \
						./src/utility/get_file_name.cpp \

ENTRY			=		./main.cpp

SRCS			=		$(UTILS) $(PARSING) $(ENTRY)

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

test_util			:
						@c++ src/utility/get_file_content.cpp -o get_file_content.out
						@c++ src/utility/is_file_accessible.cpp -o  is_file_accessible.out
						@c++ src/utility/lower_upper_str.cpp -o lower_upper_str.out
						@c++ src/utility/check_config_file.cpp -o check_config_file.out
						@c++ src/utility/strip_from_str.cpp -o strip_comment_from_file.out
						@c++ src/utility/get_file_name.cpp -o get_file_name.out
						@echo "$(P)Unit test for utilities functions built$(Reset)"

rm_util				:
						@rm *.out
						@echo "$(R)Removed utilities unit test binaries$(Reset)"

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

re				:		fclean all

.PHONY			:		clean fclean all re