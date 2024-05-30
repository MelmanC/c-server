SRC = \
	./src/server.c \
	./src/handle_file.c \

OBJ = $(SRC:.c=.o)

NAME = server

CFLAGS = -W -Wall -Wextra  -Wno-deprecated-declarations

CPPFLAGS = -I./include/

CRITF = --coverage -lcriterion

CC = gcc

DIRT = tests/test_criterion.c


all: $(NAME) clean

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	find . -name "*~" -delete
	rm -f *.gcno
	rm -f *.gcda

fclean: clean
	rm -f $(OBJ)
	rm -f $(NAME)
	make clean -C $(DIRLIB)

re: fclean compile all

compile:
	make -C $(DIRLIB)

tests: all
	gcc -o unit $(SRC) $(DIRT) $(CFLAGS) $(CPPFLAGS) $(CRITF)

debug: all
	$(CC) -g $(SRC) $(LIB) $(NAMELIB) $(CPPFLAGS)

.PHONY: all clean debug fclean re compile tests
