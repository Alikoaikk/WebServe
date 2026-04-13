NAME = webserv

CXX		= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS		= 	main.cpp server/Socket.cpp server/Client.cpp http/Request.cpp http/Response.cpp
OBJS		= $(SRCS:.cpp=.o)

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

d:
	$(CXX) $(CXXFLAGS) -g -o $(NAME) $(SRCS)

.PHONY: all clean fclean re d
