NAME    = webserv

CXX     = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

SRCS    = main.cpp \
          printing.cpp \
          src/parsing.cpp \
          src/tokenize.cpp \
          src/servParsing.cpp \
          src/locationParsing.cpp \
          src/server/Socket.cpp \
          src/server/Client.cpp \
          src/http/Request.cpp \
          src/http/Response.cpp

OBJS    = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: re
	gdb -tui ./$(NAME)

.PHONY: all clean fclean re debug
