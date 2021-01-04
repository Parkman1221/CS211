all : sixth

sixth : sixth.c
	gcc -Wall -Werror -g -fsanitize=address -std=c11 sixth.c -o sixth

clean :
	rm -f sixth

