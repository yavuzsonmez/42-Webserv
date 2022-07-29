NAME	=	webserv

SRCS	=	main.cpp \
			src/classes/configuration.cpp \
			src/classes/location.cpp \
			src/classes/server.cpp \
			src/classes/tcp_socket.cpp \

OBJS	=	$(SRCS:.cpp=.o)

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