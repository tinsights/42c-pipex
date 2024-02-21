NAME = pipex

CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Llibft -lft
INC = -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

SRCS = pipex.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJS): $(SRCS)
	cc $(CFLAGS) -c $(SRCS) $(INC)

$(LIBFT):
	make -C $(LIBDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re