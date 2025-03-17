	# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -pthread

# Source files
SRCS = 	main.c		main_process.c		utils.c		routine.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
NAME = philo

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re