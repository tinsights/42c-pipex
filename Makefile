NAME = pipex
BONUS_NAME = pipex_bonus

CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Llibft -lft
INC = -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

SRCS = pipex.c \
		input.c \
		pipex_utils.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS= $(addprefix bonus_, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	cc $(CFLAGS) -g $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJS): $(SRCS)
	cc $(CFLAGS) -c $(SRCS) $(INC)

$(LIBFT):
	make -C $(LIBDIR)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT)
	cc $(CFLAGS) -g $(BONUS_OBJS) $(LIBFLAGS) $(INC) -D BONUS=1 -o $(BONUS_NAME)

$(BONUS_OBJS): bonus_%.o: %.c
	$(CC) $(CFLAGS) $(INC) -D BONUS=1 -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re