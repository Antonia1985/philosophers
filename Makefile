NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I./include
SRC = src/main.c src/task.c src/initializers.c src/ft_atoi.c \
	src/ft_atol.c src/task_helpers.c src/monitor.c src/main_helpers.c \
	src/initializers_flags.c src/sleep.c src/task_routine.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re