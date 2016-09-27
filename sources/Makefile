# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpirsch <jpirsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/05 01:32:20 by jpirsch           #+#    #+#              #
#    Updated: 2016/09/27 04:22:11 by fjanoty          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	../hyperKube
SRC		=	drawline.c		\
			hook.c			\
			main.c			\
			cam.c			\
			hyperKube_degn.c \
			mlx_env.c

OBJ		= $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror

FLAGS-DEBUG = -g -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
		gcc -o $(NAME) $(FLAGS) $(OBJ) -I ../includes -L ../libft/ -lft \
		-L ../c_maths -lc_maths \
		-lmlx -framework OpenGL -framework AppKit
	#	-L /usr/lib/ -lmlx -lm -lX11 -lXext


$(OBJ):
	gcc -c $(FLAGS) $(SRC) -I ../includes -I ../c_maths# -I /usr/includes/

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

gdb: fclean
		gcc -c $(FLAGS-DEBUG) $(SRC) -I ../includes # -I /usr/includes/
		gcc -o $(NAME) $(FLAGS-DEBUG) $(OBJ) -L ../libft/ -lft \
		-L ../c_maths -lc_maths \
		-L /usr/lib/ -lmlx -lm -lX11 -lXext

