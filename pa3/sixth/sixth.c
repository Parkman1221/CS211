#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
	int n;//n is number of bits for ieee representation
	int expNum;//expNum is number of exponent bits
	int numBits;//numBits is number of fraction bits
	int sign;
	long number;
	int round;
	while(fscanf(f, " %d %d %d %lx %d", &n, &expNum, &numBits, &number, &round) != EOF){
		//get binary
		long integer = number;
		int* binary = malloc(64 * sizeof(int)); 
		long digit;
		long mask = 1;
		int count = 1;
		for(int i = 0; i < 64; i++){
			binary[i] = 0;
		}
		long i2 = integer;
		while(i2/2 != 0){
			count++;
			i2 /= 2;
		}
		//printf("%d\n", count);
		mask = mask << (count-1);
		//printf("%d\n", mask);
		for(int i = 0; i < count; i++){
			digit = integer & mask;
			//printf("%ld\n", integer & mask);
			if(digit != 0){
				binary[i] = 1;
			}
			else{
				binary[i] = 0;
			}
			//printf("%d\n", digit);
			mask = mask >> 1;
		}
		/*for(int i = 0; i < count; i++){
			printf("%d", binary[i]);
		}
		printf("\n");*/
		int* newBinary = malloc(64*sizeof(int));
		for(int i = 0; i < 64; i++){
			newBinary[i] = 0;
		}
		int binary1count = 0;
		for(int i = 64-count; i < 64; i++){
			newBinary[i] = binary[binary1count++];
		}
		/*for(int i = 0; i < 64; i++){
			printf("%d", newBinary[i]);
		}*/
		//take right most significant bits
		int* binary2 = malloc(n * sizeof(int));
		for(int i = 0; i < n; i++){
			binary2[i] = 0;
		}
		int binary2count = 64-count;
		binary2count = 64-n;
		for(int i = 0; i < n; i++){
			binary2[i] = newBinary[binary2count++];
		}
		/*for(int i = 0; i < n; i++){
			printf("%d", binary2[i]);
		}*/
		sign = binary2[0];
		int normal = 0;//if iterated all the way and still 0, its denormal
		for(int i = 1; i < expNum+1; i++){
			if(binary2[i] == 1){
				normal = 1;
			}
		}
		int exponentt = 0;
		int z = expNum-1;//counter to calculate the exponent
		for(int i = 1; i < expNum+1; i++){
			exponentt += binary2[i] * pow(2,z--);
		}
		//printf("\nexponent: %d\n", exponentt);
		int EE = 0;
		double inDecimal;
		if(normal == 1){ //if normal, E = exp-bias, else E = 1-bias
			EE = exponentt - (pow(2, expNum-1)-1);
			inDecimal = 1.0;
		}
		else{
			EE = 1 - (pow(2, expNum-1)-1);
			inDecimal = 0.0;
		}
		//printf("E: %d\n", EE);
		//convert 1.f to decimal for easy conversion
		int p = 1;//another counter
		for(int i = n-numBits; i < n; i++){
			inDecimal += binary2[i] * pow(2,-(p++));
		}
		//printf("in decimal: %lf\n", inDecimal);
		double finall = inDecimal * pow(2, EE);
		if(sign == 1){
			printf("-");
		}
		printf("%.*lf\n", round, finall);
		free(newBinary);
		free(binary2);
		free(binary);
	}
	fclose(f);
}

