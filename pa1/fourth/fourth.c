#include <stdio.h>
#include <stdlib.h>

int** allocate(int);

int** allocate(int m){
	/* allocate space for matrix */
	int** mat = malloc(m * sizeof(int*));
	for(int i = 0; i < m; i++){
		mat[i] = malloc(m * sizeof(int));
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			mat[i][j] = 0;
		}
	}
	mat[0][m/2] = 1;
	return mat;
}

void print(int**, int);

void print(int** mat, int m){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			printf("%d\t", mat[i][j]);
		}
		printf("\n");
	}
}

void free_matrices(int**, int);

void free_matrices(int** mat, int m){
	for(int i = 0; i < m; i++){
		free(mat[i]);
	}
	free(mat);
}

int isFinished(int**, int);

int isFinished(int** mat, int m){ /* helper method, returns 0 if matrix still not finished, 1 if finished */
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			if(mat[i][j] == 0){
				return 0;
			}
		}
	}
	return 1;
}

int** build(int**, int, int, int, int);

int** build(int** mat, int m, int r, int c, int val){ 
/* int m is the length, r and c is row and col of last placed # */
	int origr = r;
	int origc = c;
	//origr and origc used for backup rule cases
	if(r - 1 < 0){ /* wrap*/
		r = m - 1;
		if(c + 1 == m){ /* wrap*/
			c = 0;
		}
		else{
			c = c + 1;
		}
		/* rules */
		if(mat[r][c] == 0){ /* empty spot */
			mat[r][c] = val;
			return mat;
		}
		else{ /* backup rule */
			if(origr + 1 == m){ /* wrap */
				origr = 0;
				mat[origr][origc] = val;
				return mat;
			}
			else{
				mat[origr+1][origc] = val;
				return mat;
			}
		}
	}
	else if(c + 1 == m){ /* wrap*/
		c = 0;
		if(r - 1 < 0){ /* wrap */
			r = m - 1;
		}
		else{
			r = r - 1;
		}
		/* rules */
		if(mat[r][c] == 0){ /* empty spot */
			mat[r][c] = val;
			return mat;
		}
		else{ /* backup rule */
			if(origr + 1 == m){ /* wrap */
				origr = 0;
				mat[origr][origc] = val;
				return mat;
			}
			else{
				mat[origr+1][origc] = val;
				return mat;
			}
		}
	}
	else{ /* no wrap, normal rule */
		if(mat[r-1][c+1] == 0){ /* empty spot */
			mat[r-1][c+1] = val;
			return mat;
		}
		else{ /* backup rule */
			if(origr + 1 == m){ /* wrap */
				origr = 0;
				mat[origr][origc] = val;
				return mat;
			}
			else{
				mat[origr+1][origc] = val;
				return mat;
			}
		}
	}
	return mat;
}

int main(int argc, char* argv[argc+1]){
	if(argc != 2){
		printf("Please provide file name\n");
		return EXIT_SUCCESS;
	}
	
	int m;
	int val = 1;
	m = atoi(argv[1]);
	if(m % 2 == 0){ /* even number, return error */
		printf("error");
		return EXIT_SUCCESS;
	}
	int** mat = allocate(m);
	
	while(isFinished(mat, m) == 0){
		/* find r and c of largest digit in array */
		int r, c;
		for(int i = 0; i < m; i++){
			for(int j = 0; j < m; j++){
				if(mat[i][j] == val){
					r = i;
					c = j;
				}
			}
		}
		mat = build(mat, m, r, c, val+1);
		val = val + 1;
	}

	print(mat, m);
	
	free_matrices(mat, m);
	return EXIT_SUCCESS;
}
