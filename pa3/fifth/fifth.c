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
	int n;//n is number of bits for ieee representation
	int expNum;//expNum is number of exponent bits
	int numBits;//numBits is number of fraction bits
	int sign;
	/*num = -12.5;
	n = 8;
	expNum = 4;
	numBits = 3;*/
	while(fscanf(f, "%lf %d %d %d", &num, &n, &expNum, &numBits) != EOF){
		numBits += 1;
		//encode for sign bit
		if(num > 0){
			sign = 0;
		}
		else{
			sign = 1;
			num *= -1;
		}
		long integer = num;
		int* binary = malloc(64 * sizeof(int)); 
		long digit;
		long mask = 1;
		int count = 1;
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
		for(int i = 0; i < 64; i++){//changed this to i<64 from i<numBits
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
		/*for(int i = 0; i < numBits; i++){
			printf("%d", fr[i]);//fr is the fractional binary
		}*/
		//printf("\nresult\n");
		int* result = malloc(64*sizeof(int));//result is the entire number bits
		for(int i = 0; i < 64; i++){
			result[i] = 0;
		}
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
		int E; 
		int* mantissa = malloc((numBits + 3) * sizeof(int));
		for(int i = 0; i < numBits+3; i++){
			mantissa[i] = 0;
		}
		int mcount = 0;
		//if(indexfirstone < count) e = count + indexfirstone, else e = indexfirstone - count
		for(int i = 0; i < 64; i++){//change to i<64 from count+numBits
			if(result[i] == 1){
				E = count - i - 1;
				//printf("%d.", result[i]);
				for(int m = i; m < numBits+i-1+3; m++){
					//printf("%d", result[m+1]);
					mantissa[mcount++] = result[m+1];
				}
				break;
			}
		}
		/*printf("\nmantissa: ");
		for(int i = 0; i < numBits-1; i++){
			printf("%d", mantissa[i]);
		}
		printf("\n");*/
		//TAKE THE MANTISSA THAT WE ALREADY CALCULATED, MAKE ONE BIG NUMBER
		//TAKE ONE BIG NUMBER AND MAKE IT A LONG
		//ADD 1 TO THE LONG IF GRS METHOD SAYS ROUND UP
		//ELSE DO NOTHING
		//LASTLY PRINT OUT BINARY VERSION OF THAT BIG NUMBER

		//printf(" %d\n", E);
		/*int lastM = mantissa[numBits-2];
		int g = mantissa[numBits-1];
		int r = mantissa[numBits];
		int s = mantissa[numBits+1];
		printf("M: %d g: %d r: %d s: %d\n", lastM, g, r, s);
		int* newMantissa = malloc(numBits * sizeof(int));
		if((g==0) || (g==1 && r==0 && s==0 && lastM==0)){
			//round down do nothing
		}
		else{//roundup
			if(lastM == 0){
				mantissa[numBits-2] = 1;//the last mantissabit is mantissa[numBits-2]****
			}
			else{
				//convert mantissa to decimal, add 1, convert back to decimal
				int decimal = 0;
				for(int i = 0; i < numBits-1; i++){
					decimal += mantissa[i] * pow(2,numBits-i-2);
					printf("%d", mantissa[i]);
				}
				printf("\n");
				printf("\nfrac: %d\n", decimal);
				int p = pow(2,numBits-1)-1;
				if(decimal + 1 > p){
					decimal = -1;
					E++;
				}
				decimal += 1;
				count = 1;
				mask = 1;
				int deci = decimal;
				while(deci/2 != 0){
					count++;
					deci /= 2;
				}
				//count++;
				printf("\nfrac: %d\n", decimal);
				//printf("\ncount: %d\n", count);
				mask = mask << (count-1);
				//printf("\n");
				for(int i = 0; i < numBits-1; i++){//changed to i < numBits -1 // might be i<count
					digit = decimal & mask;
					printf("%d\n", digit);
					if(digit != 0){
						mantissa[i] = 1;
						//printf("1");
					}
					else{
						mantissa[i] = 0;
						//printf("0");
					}
					mask = mask >> 1;
				}
				
			}
		}*/
		/*printf("\nmantissa:");
		for(int i = 0; i < numBits+2; i++){
			printf("%d", mantissa[i]);
		}
		printf("\n");*/
		/*for(int i = 0; i < numBits-1; i++){
			printf("%d", newMantissa[i]);
		}
		printf("\n");*/
		//encode for exp bit
		int bias = pow(2, expNum-1)-1;
		//printf("\nbias: %d\n", bias);
		//printf("\nE: %d", E);
		int exp = bias + E;
		//printf("\nexp: %d\n", exp);
		int* exponent = malloc(expNum * sizeof(int));
		for(int i = 0; i < expNum; i++){
			exponent[i] = 0;
		}
		mask = 1;
		count = 1;
		int inte = exp;
		while(inte/2 != 0){
			count++;
			inte /= 2;
		}
		//printf("count: %d\n", count);//expNum = 4, count = 3. shift expNum-count + 1. 4-3+1 = 0
		if(count == expNum){
			mask = mask << (count-1);
		}
		else{
			mask = mask << ((count-1) + (expNum-count));
			count++;
		}
		for(int i = 0; i < expNum; i++){
			//printf("%d ", exp);
			digit = exp & mask;
			//printf("%lu\n", digit);
			if(digit != 0){
				exponent[i] = 1;
			}
			else{
				exponent[i] = 0;
			}
			mask = mask >> 1;
		}
		/*printf("exp:");
		for(int i = 0; i < expNum; i++){
			printf("%d", exponent[i]);
		}	
		printf("\n");*/
		int* final = malloc(n * sizeof(int));
		final[0] = sign;
		//put exp in, put mantissa in		
		for(int i = 1; i <= expNum; i++){
			final[i] = exponent[i-1];
		}
		int mantissaCount = 0;
		for(int i = expNum+1; i < n; i++){
			final[i] = mantissa[mantissaCount++];
		}
		//coonvert final to a decimal number
		long deciFloat = 0;
		int fcount = n-1;
		for(int i = 0; i < n; i++){
			deciFloat += final[i] * pow(2,fcount--);
		}
		//printf("\nnum: %lu\n", deciFloat);
		
		int lastM = mantissa[numBits-2];
		int g = mantissa[numBits-1];
		int r = mantissa[numBits];
		int s = mantissa[numBits+1];
		//printf("M: %d g: %d r: %d s: %d\n", lastM, g, r, s);
		if((g==0) || (g==1 && r==0 && s==0 && lastM==0)){
			//round down do nothing
		}
		else{//roundup
			deciFloat += 1;
		}
		//printf("\nfinal num: %lu\n", deciFloat);
		//now convert deciFloat back to binary
		mask = 1;
		count = 1;
		//get binary
		long i3 = deciFloat;
		while(i3/2 != 0){
			count++;
			i3 /= 2;
		}
		//printf("\n%d\n", count);
		if(sign == 0){
			count++;
		}
		//printf("count:%d\n", count);
		mask = mask << (count-1);
		//printf("%lu\n", mask);
		for(int i = 0; i < n-count; i++){
			final[i] = 0;
		}
		for(int i = n-count; i < n; i++){
			digit = deciFloat & mask;
			//printf("%ld\n", deciFloat & mask);
			if(digit != 0){
				final[i] = 1;
				//printf("1");
			}
			else{
				//printf("0");
				final[i] = 0;
			}
			//printf("%d\n", digit);
			mask = mask >> 1;
		}
		for(int i = 0; i < n; i++){
			printf("%d", final[i]);
		}
		printf("\n");
		//free(newMantissa);
		free(mantissa);
		free(final);
		free(exponent);
		free(binary);
		free(fr);
		free(result);

	}
	fclose(f);
}

