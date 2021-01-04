#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[argc+1]){
	if(argc != 6){
		printf("error\n");
		return EXIT_SUCCESS;
	}
	FILE* f = fopen(argv[5], "r");
	if(f == 0){
		printf("error\n");
		return EXIT_SUCCESS;
	}
	int cacheSize = atoi(argv[1]); //cachesize = #sets * #blocks within set * #bytes in block
	int assoc; //associativity = # blocks within a set
	int numSets;
	int setBits;
	int blockBits;
	//int tagBits;
	int blockSize = atoi(argv[4]);	
	if(strcmp(argv[2], "direct") == 0){
		assoc = 1; //1 block per set
		numSets = cacheSize / (blockSize * assoc);
		blockBits = log2(blockSize);
		setBits = log2(numSets);
	}
	else if(strcmp(argv[2], "assoc") == 0){
		numSets = 1;
		assoc = cacheSize / (blockSize * numSets); //all blocks in 1 set
		blockBits = log2(blockSize);
		setBits = 0;
	}
	else{//assoc:n
		assoc = atoi(&(argv[2][6])); //n blocks per set
		numSets = cacheSize / (blockSize * assoc);
		blockBits = log2(blockSize);
		setBits = log2(numSets);
	}
	int policy; //policy 0 is FIFO, policy 1 is LRU
	if(strcmp(argv[3], "fifo") == 0){
		policy = 0;
	}
	else{
		policy = 1;
	}
	//CHECK FOR VALID INPUTS
	if(cacheSize == 0 || ceil(log2(cacheSize)) != floor(log2(cacheSize))){
		printf("error\n");
		return EXIT_SUCCESS;
	}
	if(ceil(log2(blockSize)) != floor(log2(blockSize))){
		printf("error\n");
		return EXIT_SUCCESS;
	}

	//tagBits = 48-setBits-blockBits;
	//printf("cachesize: %d\nassoc: %d\npolicy: %d\nblocksize: %d\n\n", cacheSize, assoc, policy, blockSize);
	//printf("numsets: %d\ntagBits: %d\nsetBits: %d\nblockBits: %d\n\n", numSets, tagBits, setBits, blockBits);
	
	//create cache, an array of sets
	long** cache = malloc(numSets*assoc*sizeof(long*));
	//create the array of blocks in the set
	for(int i = 0; i < numSets*assoc; i++){
		cache[i] = malloc(3*sizeof(long));
		cache[i][0] = 0; //valid bit
		cache[i][1] = 0; //tag bit
		cache[i][2] = 0; //holder bit
	}
	/*for(int i = 0; i < numSets*assoc; i++){
		printf("%lu %lu %lu\n", cache[i][0], cache[i][1], cache[i][2]);
	}*/
	long memRead=0;
	long memWrite=0;
	long cacheHit=0;
	long cacheMiss=0;

	char op[20];//read or write
	long int input;
	while(fscanf(f, "%s %lx\n", op, &input) != EOF){
		//printf("%c %lu", op, input);
		if(strcmp(op,"W") == 0){
			memWrite++;
		}
		long temp = input >>  blockBits;
		int setIndex = temp & ((1<<setBits)-1);
		unsigned long tag = input >> (blockBits + setBits);
		
		int hit = 0; //if cacheHit = 0, its a cache miss. else its a cache hit
		for(int i = setIndex * assoc; i < (setIndex*assoc)+assoc; i++){ 
			//index in cache = setIndex*assoc, traverse length of 1 set
			if(cache[i][0] == 1 && cache[i][1] == tag){ //if VALID 1 AND TAG IS EQUAL CACHE HIT
				hit = 1;
			}
		}
//for LRU, if you increment everything, and set the block you used to 0, its basically updating
//when updating cache, take the largest
//for FIFO, increment every block every time, when updating take the largest
		if(hit == 1){//if cache hit, just update counter
			cacheHit++;
			if(policy == 0){//FIFO: increment all blocks
				for(int i = setIndex*assoc; i < (setIndex*assoc)+assoc; i++){
					if(cache[i][0] == 1){
						cache[i][2]++;
					}
				}
			}
			else{//LRU: increment everything, set the hit block to 0
				for(int i = setIndex*assoc; i < (setIndex*assoc)+assoc;i++){
					if(cache[i][0] == 1){
						cache[i][2]++;
					}
					if(cache[i][0] == 1 && cache[i][1] == tag){//this is the hit block
						cache[i][2] = 0;
					}
				}
			}
		}
		else{//if cache miss, update cache
			cacheMiss++;
			memRead++;
			//policy 0 = fifo, policy 1 = LRU
			/*
			INCREMENT ALL
			- check if set has empty block, same search as finding if cachehit/miss
			- if set has empty block, set cache[emptyblock][0] = 1, cache[tmpyblock][1] = tag
			- if set is full, then
					search and find block with MAX age
					valid = 1, tag = tag, age = 0
			*/
			for(int i = setIndex*assoc; i < (setIndex*assoc)+assoc; i++){//increment all
				if(cache[i][0] == 1){
					cache[i][2]++;
				}
			}
			int evictSetIndex = -1;
			for(int  i = setIndex*assoc; i< (setIndex*assoc)+assoc; i++){
				if(cache[i][0] == 0){//found an empty block, break
					evictSetIndex = i;
					break;
				}
			}
			if(evictSetIndex >= 0){//evictSetIndex is >= 0 if block was found, else have to evict
				cache[evictSetIndex][0] = 1;
				cache[evictSetIndex][1] = tag;
				cache[evictSetIndex][2] = 0;
			}
			else{//set is full, find max age, and update
				int oldest = 0;
				for(int i = setIndex*assoc; i < (setIndex*assoc)+assoc;i++){
					if(cache[i][0] == 1 && cache[i][2] > oldest){
						oldest = cache[i][2];
						evictSetIndex = i;
					}
				}
				//update
				cache[evictSetIndex][0] = 1;
				cache[evictSetIndex][1] = tag;
				cache[evictSetIndex][2] = 0;
			}
		}
	}
	printf("memread: %lu\nmemwrite: %lu\ncachehit: %lu\ncachemiss: %lu\n", memRead, memWrite, cacheHit, cacheMiss);


	fclose(f);
	for(int i = 0; i < numSets*assoc; i++){
		free(cache[i]);
	}
	free(cache);
}
