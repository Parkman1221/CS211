all : sixth

sixth : sixth.c
	gcc -Wall -Werror -fsanitize=address -std=c11 -g sixth.c -o sixth -lm

clean :
	rm -f sixth

