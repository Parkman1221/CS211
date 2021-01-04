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
	double num;
	int numBits;
	while(fscanf(f, "%lf %d", &num, &numBits) != EOF){
		numBits++;
		long integer = num;
		int* binary = malloc(64 * sizeof(int)); 
		int digit;
		int mask = 1;
		int count = 1;
		int b = 0;
		//get binary
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
			b += digit;
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
			printf("%d", binary[i]);//result is the integrand binary
		}
		printf("\n");*/
		int* fr = malloc(64 * sizeof(int)); //fr is fraction bits
		for(int i = 0; i < 64; i++){
			fr[i] = 0;
		}
		double fraction = num - integer;
		for(int i = 0; i < 64; i++){
			fraction *= 2;
			int fractionBit = fraction;
			if(fractionBit == 1){
				fraction  -= fractionBit;
				fr[i] = 1;
			}
			else{
				fr[i] = 0;
			}
		}
		/*printf("\n");
		for(int i = 0; i < 64; i++){
			printf("%d", fr[i]);//fr is the fractional binary
		}*/
		//printf("\nresult\n");
		int* result = malloc(64*sizeof(int));//result is the entire number bits
		for(int i = 0; i < count; i++){
			result[i] = binary[i];
		}
		int a = 0;
		for(int i = count; i < 64; i++){
			result[i] = fr[a++];
		}
		/*for(int i = 0; i < 64; i++){
			printf("%d", result[i]);
		}*/

		if(result[0] == 0){
			//find the first 1
			int EE = 0;
			for(int i = 1; i < 64; i++){
				if(result[i] == 1){
					EE-=1;
					printf("%d.", result[i]);
					for(int m = i; m < numBits+i-1; m++){
						printf("%d", result[m+1]);
					}
					break;
				}	
				EE -= 1;
			}
			printf(" %d\n", EE);
		}
		else{
			int E; 
			//if(indexfirstone < count) e = count + indexfirstone, else e = indexfirstone - count
			for(int i = 0; i < count+numBits; i++){
				if(result[i] == 1){
					E = count - i;
					printf("%d.", result[i]);
					for(int m = i; m < numBits+i-1; m++){
						printf("%d", result[m+1]);
					}
					break;
				}
			}
			printf(" %d\n", E-1);
		}
		free(binary);
		free(fr);
		free(result);

	}
	fclose(f);
}

