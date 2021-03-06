#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void toBinary(long num, int numBits){
	int* binary = malloc(numBits * sizeof(int)); //i have this separate array just in case its larger than numBits
	for(int i = 0; i < numBits; i++){
		binary[i] = 0;
	}
	int count = 0;
	long temp = pow(2,numBits);
	if(num > temp - 1){
		num = num % temp;
	}
	while(num > 0){
		if(num == 1){
			binary[count] = 1;
			break;
		}
		if(num == 0){
			binary[count] = 0;
			break;
		}
		binary[count] = num % 2;
		num = num / 2;
		count++;
	}
	for(int i = numBits-1; i >= 0; i--){
		printf("%d", binary[i]);
	}
	printf("\n");
	free(binary);
}

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
	long num;
	int numBits;
	while(fscanf(f, "%ld %d\n", &num, &numBits) != EOF){
		toBinary(num, numBits);
		//printf("%d, %d\n", num, numBits);
	}
	fclose(f);
}
