#include <stdio.h>
#include <stdlib.h>

int** read_matrix(FILE*, int*);

int** read_matrix(FILE* f, int* m1){
	int m;
	fscanf(f, "%d\n", &m);
	*m1 = m;

	/* allocate space for matrix */
	int** mat = malloc(m * sizeof(int*));
	for(int i = 0; i < m; i++){
		mat[i] = malloc(m * sizeof(int));
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			fscanf(f, "%d", &mat[i][j]);
		}
		fscanf(f, "\n");
	}
	return mat;
}

void free_matrices(int**, int);

void free_matrices(int** mat, int m){
	for(int i = 0; i < m; i++){
		free(mat[i]);
	}
	free(mat);
}

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

int** pivot(int**, int**, int, int);

int** pivot(int** mat, int** temp, int m, int i){ //m is size of arr, i is col to be removed
	/* set the row to be deleted to 0s */
	int smallR = 0;
	int smallC = 0;
	for(int r = 0; r < m; r++){
		for(int c = 0; c < m; c++){
			if(r != 0 && c != i){ //copy mat to temp
				temp[smallR][smallC++] = mat[r][c]; //increment smallC AFTER copying
				//check if smallR matrix is filled, then reset smallC, increment smallR
				if(smallC == m-1){
					smallC = 0;
					smallR++;
				}
			}
		}
	}
	return temp;
}

int det(int**, int);

int det(int** mat, int m){ /* recursive function, m is size of array */
	int result = 0;
	if(m == 1){ /* base case, 1x1 array */
		//free here
		int temp = mat[0][0];
		free_matrices(mat, 1);
		return temp;
	}
	int alt = 1;
	for(int i = 0; i < m; i++){
		//allocate new smaller matrix
		int** temp = allocate(m-1);
		//fill smaller matrix
		temp = pivot(mat, temp, m, i);
		//calculate determinant
		result += alt * (mat[0][i] * det(temp, m-1));
		alt = -alt;
	}
	free_matrices(mat, m);
	return result;
}

int main(int argc, char* argv[argc+1]){
	if(argc != 2){
		printf("Please provide file name\n");
		return EXIT_SUCCESS;
	}
	
	FILE* f = fopen(argv[1], "r");
	int m1;
	int** mat = read_matrix(f, &m1);
	int result = det(mat, m1);

	printf("%d", result);
	
	fclose(f);
	return EXIT_SUCCESS;
}
