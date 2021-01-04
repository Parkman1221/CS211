#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int getSetIndex(long address, int blockBits, int setBits){
	long temp = address >>  blockBits;
	int setIndex = temp & ((1L<<setBits)-1);
	return setIndex;
}

int main(int argc, char* argv[argc+1]){
	if(argc != 9){
		printf("error\n");
		return EXIT_SUCCESS;
	}
	FILE* f = fopen(argv[8], "r");
	if(f == 0){
		printf("error\n");
		return EXIT_SUCCESS;
	}
	int cacheSize = atoi(argv[1]); //cachesize = #sets * #blocks within set * #bytes in block
	int assoc; //associativity = # blocks within a set
	int numSets;
	int setBits;
	int blockBits;
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
	int cacheSize2 = atoi(argv[5]);
	int assoc2;
	int numSets2;
	int setBits2;
	int blockBits2;
	int blockSize2 = blockSize;
	if(strcmp(argv[6], "direct") == 0){
		assoc2 = 1; //1 block per set
		numSets2 = cacheSize2 / (blockSize2 * assoc2);
		blockBits2 = log2(blockSize2);
		setBits2 = log2(numSets2);
	}
	else if(strcmp(argv[6], "assoc") == 0){
		numSets2 = 1;
		assoc2 = cacheSize2 / (blockSize2 * numSets2); //all blocks in 1 set
		blockBits2 = log2(blockSize2);
		setBits2 = 0;
	}
	else{//assoc:n
		assoc2 = atoi(&(argv[6][6])); //n blocks per set
		numSets2 = cacheSize2 / (blockSize2 * assoc2);
		blockBits2 = log2(blockSize2);
		setBits2 = log2(numSets2);
	}
	/*int policy2;
	if(strcmp(argv[7], "fifo") == 0){
		policy2 = 0;
	}
	else{
		policy2 = 1;
	}*/
	//CHECK FOR VALID INPUTS
	if(cacheSize == 0 || ceil(log2(cacheSize)) != floor(log2(cacheSize)) || cacheSize2 == 0 || ceil(log2(cacheSize2)) != floor(log2(cacheSize2))){
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
		cache[i][2] = 0; //age bit
	}
	long** cache2 = malloc(numSets2*assoc2*sizeof(long*));
	for(int i = 0; i < numSets2*assoc2; i++){
		cache2[i] = malloc(3*sizeof(long));
		cache2[i][0] = 0;
		cache2[i][0] = 0;
		cache2[i][0] = 0;
	}
	/*for(int i = 0; i < numSets*assoc; i++){
		printf("%lu %lu %lu\n", cache[i][0], cache[i][1], cache[i][2]);
	}*/
	long memRead=0;
	long memWrite=0;
	long cacheHit=0;
	long cacheMiss=0;
	long cache2Hit=0;
	long cache2Miss=0;
	long total = 0;
	char op[20];//read or write
	long int input;
	while(fscanf(f, "%s %lx\n", op, &input) != EOF){
		//printf("%c %lu", op, input);
		if(strcmp(op,"W") == 0){
			memWrite++;
		}
		assert(cacheHit+cacheMiss==total);
		total++;
		unsigned long tag = input >> (blockBits + setBits);
		//long temp2 = input >> blockBits2;
		//int setIndex2 = temp2 & ((1<<setBits2)-1);
		//unsigned long tag2 = input >> (blockBits2 + setBits2);
		int setIndex = getSetIndex(input, blockBits, setBits);
		int hit = 0; //if cacheHit = 0, its a cache miss. else its a cache hit
		for(int i = setIndex*assoc; i < (setIndex*assoc)+assoc; i++){ 
			//index in cache = setIndex*assoc, traverse length of 1 set
			if(cache[i][0] == 1 && ((cache[i][1]>>(blockBits+setBits)) == tag)){ //if VALID 1 AND TAG IS EQUAL CACHE HIT LEVEL 1
				hit = 1;
				break;
			}
		}
//for LRU, if you increment everything, and set the block you used to 0, its basically updating
//when updating cache, take the largest
//for FIFO, increment every block every time, when updating take the largest
		if(hit == 1){//if cache hit, just update counter
			cacheHit++;
			if(policy == 0){//FIFO: increment all blocks
				for(int i = 0; i < numSets*assoc; i++){
					if(cache[i][0] == 1){
						cache[i][2]++;
					}
				}
			}
			else{//LRU: increment everything, set the hit block to 0
				for(int i = 0; i < numSets*assoc;i++){
					if(cache[i][0] == 1){
						cache[i][2]++;
					}
					if(cache[i][0] == 1 && ((cache[i][1]>>(blockBits+setBits)) == tag)){//this is the hit block
						cache[i][2] = 0;
					}
				}
			}
			/*if(policy2 == 0){//FIFO: increment all blocks
				for(int i = 0; i < numSets2*assoc2; i++){
					if(cache2[i][0] == 1){
						cache2[i][2]++;
					}
				}
			}
			else{//LRU: increment everything, set the hit block to 0
				for(int i = 0; i < numSets2*assoc2;i++){
					if(cache2[i][0] == 1){
						cache2[i][2]++;
					}
					if(cache2[i][0] == 1 && cache2[i][1]>>(blockBits2+setBits2) == tag2){//this is the hit block
						cache2[i][2] = 0;
					}
				}
				
			}*/
			
		}
		else{//if cache miss, update cache
			cacheMiss++;
			//policy 0 = fifo, policy 1 = LRU
			/*
			INCREMENT ALL
			- check if set has empty block, same search as finding if cachehit/miss
			- if set has empty block, set cache[emptyblock][0] = 1, cache[tmpyblock][1] = tag
			- if set is full, then
					search and find block with MAX age
					valid = 1, tag = tag, age = 0
			*/
			/*
			L2 cache steps
			INCREMENT BOTH CACHES
			check if set2 has a hit
			if set2 hit
				cache2hit++
				store cache2tag
				if cache1 set is full, then EVICT
					search and find block1 with MAX age
					save max age's --- tempTag = cache1tag
					cache1 valid = 1, tag = cache2tag, age = 0
					search through cache2, if cache2 has empty block, cache2[empty][0] =1, cache2[empty][1] = tempTag
					if cache2 is full, then
						search and find block with MAX age
						valid = 1, tag = tempTag, age = 0
				if cache1 set not full, then set cache1[empty][0] = 1, cache1[empty][1] = cache2tag
			if set2 miss
				cache2miss++
				if cache1set is full, then EVICT
					search and find block with MAX age
					save max age's ---- tempTag = cache1tag
					cache1 valid = 1, tag = tag, age = 0
					search through cache2, if cache2 has empty block, cache2[empty][0] = 1, cache2[empty][1] = tempTag
					if cache2 full, then
						search and find block wit MAX age
						valid = 1, tag = tempTag, age = 0
				if cache1set not full, then
					set cache1[empty][0] = 1, cache1[empty][1] = tag
					
				
			*/
			for(int i = 0; i < (numSets*assoc); i++){//increment all in cache1
				if(cache[i][0] == 1){
					cache[i][2]++;
				}
			}
			//now search in cache2 to find a hit
			int hit2 = 0; //if cacheHit2 = 0, its a cache miss. else its a cache hit
			unsigned long cache2Tag;
			int ttempSetIndex2 = getSetIndex(input, blockBits2, setBits2);
			for(int i = ttempSetIndex2*assoc2; i < (ttempSetIndex2*assoc2)+assoc2; i++){ 
				if(cache2[i][0] == 1 && (cache2[i][1]>>(blockBits2+setBits2) == (input>>(blockBits2+setBits2)))){//cache2 hit
					hit2 = 1;
					cache2Tag = cache2[i][1];
					cache2[i][0] = 0;
					cache2[i][1] = 0;
					cache2[i][2] = 0;
					break;
				}
			}
			if(hit2 == 1){
				cache2Hit++;
				//increment all blocks in cache2
				for(int i = 0; i < numSets2*assoc2; i++){
					if(cache2[i][0] == 1){
						cache2[i][2]++;
					}	
				}
				//search cache1
				int evictSetIndex = -1;
				int tempSetIndex = getSetIndex(cache2Tag, blockBits, setBits);
				for(int i = tempSetIndex*assoc; i < (tempSetIndex*assoc)+assoc; i++){
					if(cache[i][0] == 0){//found an empty block in cache1, break
						evictSetIndex = i;
						break;
					}
				}
				if(evictSetIndex >= 0){//evictSetIndex is >= 0 if CACHE1 was found, else have to evict, CACHE NOT FULL
					cache[evictSetIndex][0] = 1;
					cache[evictSetIndex][1] = cache2Tag;
					cache[evictSetIndex][2] = 0;
					
				}
				else{//CACHE1 is full, find max age, and update
					int oldest = 0;
					unsigned long cache1Tag;
					for(int i = tempSetIndex*assoc; i < (tempSetIndex*assoc)+assoc;i++){
						if(cache[i][0] == 1 && cache[i][2] > oldest){
							oldest = cache[i][2];
							evictSetIndex = i;
						}
					}
					//update cache1
					cache1Tag = cache[evictSetIndex][1];
					cache[evictSetIndex][0] = 1;
					cache[evictSetIndex][1] = cache2Tag;
					cache[evictSetIndex][2] = 0;
					
					//now update cache2
					int evictSetIndex2 = -1;
					int tempSetIndex2 = getSetIndex(cache1Tag, blockBits2, setBits2);
					for(int i = tempSetIndex2*assoc2; i < (tempSetIndex2*assoc2)+assoc2; i++){
						if(cache2[i][0] == 0){
							evictSetIndex2 = i;
							break;
						}
					}
					if(evictSetIndex2 >= 0){
						cache2[evictSetIndex2][0] = 1;
						cache2[evictSetIndex2][1] = cache1Tag;
						cache2[evictSetIndex2][2] = 0;
					}
					else{//cache 2 is full
						int oldest2 = 0;
						for(int i = tempSetIndex2*assoc2; i < (tempSetIndex2*assoc2)+assoc2;i++){
							if(cache2[i][0] == 1 && cache2[i][2] > oldest2){
								oldest2 = cache2[i][2];
								evictSetIndex2 = i;
							}
						}
						//update
						cache2[evictSetIndex2][0] = 1;
						cache2[evictSetIndex2][1] = cache1Tag;
						cache2[evictSetIndex2][2] = 0;
					}
				}
				
			}
			else{ //cache2miss
			/*
			if set2 miss
				cache2miss++
				if cache1set is full, then EVICT
					search and find block with MAX age
					save max age's ---- tempTag = cache1tag
					cache1 valid = 1, tag = tag, age = 0
					search through cache2, if cache2 has empty block, cache2[empty][0] = 1, cache2[empty][1] = tempTag
					if cache2 full, then
						search and find block wit MAX age
						valid = 1, tag = tempTag, age = 0
				if cache1set not full, then
					set cache1[empty][0] = 1, cache1[empty][1] = tag*/
				cache2Miss++;
				memRead++;
				for(int i = 0; i < numSets2*assoc2; i++){ //increment all
					if(cache2[i][0] == 1){
						cache2[i][2]++;
					}
				}
				//search cache1
				int evictSetIndex = -1;
				int tempSetIndex = getSetIndex(input, blockBits, setBits);
				for(int i = tempSetIndex*assoc; i < (tempSetIndex*assoc)+assoc; i++){
					if(cache[i][0] == 0){//found an empty block in cache1, break
						evictSetIndex = i;
						break;
					}
				}
				if(evictSetIndex >= 0){//evictSetIndex is >= 0 if CACHE1 was found, else have to evict, CACHE NOT FULL
					cache[evictSetIndex][0] = 1;
					cache[evictSetIndex][1] = input;
					cache[evictSetIndex][2] = 0;
				}
				else{//CACHE1 is full, find max age, and update
					int oldest = 0;
					unsigned long cache1Tag;
					for(int i = tempSetIndex*assoc; i < (tempSetIndex*assoc)+assoc; i++){
						if(cache[i][0] == 1 && cache[i][2] > oldest){
							oldest = cache[i][2];
							evictSetIndex = i;
						}
					}
					//update cache1
					cache1Tag = cache[evictSetIndex][1];
					cache[evictSetIndex][0] = 1;
					cache[evictSetIndex][1] = input;
					cache[evictSetIndex][2] = 0;
					//now update cache2
					int tempSetIndex2 = getSetIndex(cache1Tag, blockBits2, setBits2);
					int evictSetIndex2 = -1;
					for(int i = tempSetIndex2*assoc2; i < (tempSetIndex2*assoc2)+assoc2; i++){
						if(cache2[i][0] == 0){
							evictSetIndex2 = i;
							break;
						}
					}
					if(evictSetIndex2 >= 0){
						cache2[evictSetIndex2][0] = 1;
						cache2[evictSetIndex2][1] = cache1Tag;
						cache2[evictSetIndex2][2] = 0;
					}
					else{//cache 2 is full
						int oldest2 = 0;
						for(int i = tempSetIndex2*assoc2; i < (tempSetIndex2*assoc2)+assoc2;i++){
							if(cache2[i][0] == 1 && cache2[i][2] > oldest2){
								oldest2 = cache2[i][2];
								evictSetIndex2 = i;
							}
						}
						//update
						cache2[evictSetIndex2][0] = 1;
						cache2[evictSetIndex2][1] = cache1Tag;
						cache2[evictSetIndex2][2] = 0;
					}
				}
			}
		}
	}
	printf("memread: %lu\nmemwrite: %lu\nl1cachehit: %lu\nl1cachemiss: %lu\nl2cachehit: %lu\nl2cachemiss: %lu\n", memRead, memWrite, cacheHit, cacheMiss, cache2Hit, cache2Miss);


	fclose(f);
	for(int i = 0; i < numSets*assoc; i++){
		free(cache[i]);
	}
	for(int i = 0; i < numSets2*assoc2; i++){
		free(cache2[i]);
	}
	free(cache2);
	free(cache);
	return EXIT_SUCCESS;
}
