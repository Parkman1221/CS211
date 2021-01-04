#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void utos(long num, int numBits){
	//unsigned to signed
	long mask = 1;
	long result = 0;
	long digit;
	for(int i = 0; i < numBits; i++){
		digit = num & mask;
		if(i == numBits-1){
			if(digit != 0){
				result -= digit;
			}
			else{
				result += digit;
			}
		}
		else{
			result += digit;
		}
		mask = mask << 1;		
	}
	printf("%ld", result);
}
void stou(long num, int numBits){
	//signed to unsigned
	long mask = 1;
	long result = 0;
	long digit;
	for(int i = 0; i < numBits; i++){
		digit = num & mask;
		result += digit;
		mask = mask << 1;		
	}
	printf("%ld", result);
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
	char first;
	char second;
	while(fscanf(f, "%lu %d %c %c", &num, &numBits, &first, &second) != EOF){
		if(first == 'u'){
			utos(num, numBits);
		}
		else{
			stou(num, numBits);
		}
		printf("\n");
	}
	/*
	int digit;
	int mask = 1;
	//print binary
	for(int i = 0; i < numBits; i++){
		digit = num & mask;
		if(digit != 0){
			printf("1");
		}
		else{
			printf("0");
		}
		//printf("%d\n", digit);
		mask = mask << 1;
	}
	printf("\n");*/
	fclose(f);
}
