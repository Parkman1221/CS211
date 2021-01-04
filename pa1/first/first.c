#include <stdlib.h>
#include <stdio.h>

int isPrime(int num);

int main(int argc, char* argv[argc+1]){

	if(argc != 2){
	printf("Please provide file name\n");
	return EXIT_SUCCESS;
	}

	FILE* f = fopen(argv[1], "r");
	if(f == 0){
	printf("error");
	return EXIT_SUCCESS;
	}
	/*char content[100];*/
	int input;
	while(fscanf(f, "%d\n", &input) != EOF){
		if(input == 0 || input == 1){
			printf("no\n");
			continue;
		}
		if(isPrime(input) == 0){
			printf("no\n");
			continue;
		}
		else if(input - 2 < 0){
			if(isPrime(input + 2) == 1){
				printf("yes\n");
			}
			else{
				printf("no\n");
			}
		}
		else{
			if(isPrime(input-2) == 1 || isPrime(input+2) == 1)
				printf("yes\n");
			else
				printf("no\n");
		}
	}
	fclose(f);
	return EXIT_SUCCESS;
}

int isPrime(int num){
	if(num == 1){
		return 0;
	}
	for(int i = 2; i < num; i++){
		if(num % i == 0){
			return 0;
		}
	}
	return 1;
}















