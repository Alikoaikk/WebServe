NAME    = webserv

CXX     = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

SRCS    = main.cpp \
          src/parsing.cpp \
          src/tokenize.cpp \
          src/servParsing.cpp \
          src/locationParsing.cpp

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
