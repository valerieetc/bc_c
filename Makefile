all: my_bc

my_bc: my_bc.o
	gcc -Wall -Wextra -Werror -o my_bc my_bc.o


my_bc.o: my_bc.c
	gcc -Wall -Wextra -Werror -c my_bc.c

clean:
	rm -f *.o

fclean: clean
	rm -f my_bc

re: fclean all