NAME	=	webserv

SRCS	=	main.cpp \
			src/classes/cgi.cpp \
			src/classes/configuration.cpp \
			src/classes/location.cpp \
			src/classes/response.cpp \
			src/classes/server.cpp \
			src/classes/tcp_socket.cpp \

UTIL	=	src/utility/check_config_file.cpp \
			src/utility/filter_characters.cpp \
			src/utility/get_file_content.cpp \
			src/utility/get_file_name.cpp \
			src/utility/is_file_accessible.cpp \
			src/utility/lower_upper_str.cpp \
			src/utility/strip_from_str.cpp \
			src/utility/to_str.cpp \
			src/utility/validate_parantheses.cpp \

OBJS	=	$(SRCS:.cpp=.o) $(UTIL:.cpp=.o)

FLAGS	=	-Wall -Werror -Wextra -std=c++98

.c.o	:
			c++ $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME)	:	$(OBJS)
			c++ $(FLAGS) $(OBJS) -o $(NAME)

clean	:
			rm -f $(OBJS)

fclean	:	clean
			rm -f $(NAME)

all		:	$(NAME)

re		:	fclean all

.PHONY	:	clean fclean all re