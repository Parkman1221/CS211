all : fourth

fourth : fourth.c
	gcc -Wall -Werror -fsanitize=address -std=c11 -g fourth.c -o fourth -lm

clean :
	rm -f fourth

