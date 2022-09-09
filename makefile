Reset			=		\033[0m						# Text Reset
R				=		\033[0;31m					# Red
G				=		\033[0;32m					# Green
B				=		\033[0;34m					# Blue
P				=		\033[0;35m					# Purple
Y				=		\033[33;1m					# Yellow

NAME			=		webserv

HDRS			=		./inc/config_file/ConfigFileParsing.hpp \
						./inc/configuration_key/ConfigurationKey.hpp \
						./inc/configuration_key/ServerBlock.hpp \
						./inc/debugger/Singleton.hpp \
						./inc/debugger/DebuggerPrinter.hpp \
						./inc/http/headers.hpp \
						./inc/http/Request.hpp \
						./inc/http/Response.hpp \
						./inc/http/status.hpp \
						./inc/network/ClientSocket.hpp \
						./inc/network/Multiplexer.hpp \
						./inc/network/ServerSocket.hpp \
						./inc/utility/colors.hpp \
						./inc/utility/utility.hpp \

ENTRY			=		./main.cpp

DEBUGGER		=		./src/debugger/DebuggerPrinter.cpp \

CONFIG_FILE		=		./src/config_file/ConfigFileParsing.cpp \
						./src/configuration_key/ConfigurationKey.cpp \
						./src/configuration_key/ServerBlock.cpp \

HTTP			=		./src/http/Request.cpp \
						./src/http/Response.cpp \
						./src/http/Cgi.cpp \

NETWORK			=		./src/network/ClientSocket.cpp \
						./src/network/Multiplexer.cpp \
						./src/network/ServerSocket.cpp \

UTILS			=		./src/utility/get_file_content.cpp \
						./src/utility/is_file_accessible.cpp \
						./src/utility/lower_upper_str.cpp \
						./src/utility/check_config_file.cpp \
						./src/utility/filter_characters.cpp \
						./src/utility/strip_from_str.cpp \
						./src/utility/get_file_name.cpp \
						./src/utility/get_server_block.cpp \
						./src/utility/validate_parantheses.cpp \
						./src/utility/split_on_delimiter.cpp \
						./src/utility/convert_configuration_key_type.cpp \
						./src/utility/split_string.cpp \

SRCS			=		$(ENTRY) $(DEBUGGER) $(CONFIG_FILE) $(HTTP) $(NETWORK) $(UTILS)

OBJS			=		$(SRCS:.cpp=.o)

DEBUG			=		-g -fsanitize=address

FLAGS			=		-Wall -Werror -Wextra -Wshadow -Wno-shadow -std=c++98 -g

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

re				:		fclean all

.PHONY			:		clean fclean all re
