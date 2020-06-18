#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "prototype_s.h"


//allocate and matrix nxm
MAT* mat_create_with_type(unsigned int rows_mat, unsigned int cols_mat) {
	MAT* M = malloc(sizeof(MAT));
	M->rows = rows_mat;
	M->cols = cols_mat;
	M->elem = (float*)malloc(sizeof(float) * rows_mat * cols_mat);
	if (M->elem == NULL) {
		mat_destroy(M);
		return NULL;
	}
	return M;
}

//Save matrix from memory to file with noticed name
char mat_save(MAT* mat, char* filename) {
	int fw;
	unsigned int i = 0;
	float elem_matici = 0;

	if ((fw = open(filename, O_WRONLY)) == -1) {
		return NULL;
	}

	write(fw, "M1", 2*sizeof(char));
	write(fw, &mat->rows, sizeof(unsigned int));
	write(fw, &mat->cols, sizeof(unsigned int));
	write(fw, mat->elem, mat->rows * mat->cols * sizeof(float));

	return 0;
}


//vypise maticu mat na obrazovku z memory print on screen matrix from memory
void mat_print(MAT* mat) {
	unsigned int i;

	for (i = 0; i < mat->rows * mat->cols; i++) {
		printf("%f\t", mat->elem[i]);
		if (i % mat->rows == mat->rows - 1)
			printf("\n");
	}
}

//free allocate memory
void mat_destroy(MAT* mat) {
	free(mat->elem);
	free(mat);
}

//nastavuje elementy matice na nahodne hodnoty v rozmedzi od −1 po +1
void mat_random(MAT* mat) {
	unsigned int i;

	for (i = 0; i < mat->rows * mat->cols; i++) {
		mat->elem [i] = (float)(-1 + 2.0 * rand() / (double)(RAND_MAX + 1));
	}
}

//inicializuje hustu maticu mat tak, aby vsetky mimodiagonalne elementy boli nulove a vsetky diagonalne elementy jednotkove
void mat_unit(MAT* mat) {
	unsigned int i,j;

	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			if (i == j)
				ELEM(mat, i, j) = 1;
			else
				ELEM(mat, i, j) = 0;
		}
	}
}

//read matrix from file into memory
MAT* mat_create_by_file(char* filename) {
	int fr;
	unsigned int rows_m = 0, cols_m = 0, i = 0;
	MAT* p_M;
	float temp_elem = 0;

	if ((fr = open(filename, O_RDONLY)) == -1) {
		return NULL;
	}

	char ch[2] = {0};

	read(fr, &ch[0], sizeof(char));
	read(fr, &ch[1], sizeof(char));
	read(fr, &rows_m, sizeof(unsigned int));
	read(fr, &cols_m, sizeof(unsigned int));
	p_M = mat_create_with_type(rows_m, cols_m);
	read(fr, p_M->elem, rows_m * cols_m * sizeof(float));

	close(fr);
	return p_M;
}


void mix_array_of_permutation(unsigned int* array_permutation, unsigned int dim) {
	unsigned int i, j=0, temp_swap;

	for (i = dim - 1; i >= 1; i--) {
		int j = rand() % (i + 1);
		temp_swap = array_permutation[j];
		array_permutation[j] = array_permutation[i];
		array_permutation[i] = temp_swap;
	}
}

//generuje randomnu bistochastic matrix
char mat_create_random_bistochastic(MAT* mat) {
	unsigned int i, j, k;
	float sum_matrix_multiplikator = 0;

	unsigned int* permutac;
	float* matrix_multiplikator;

	if (mat->rows != mat->cols)
		return 1;

	//allocate mamory for array of permutation
	permutac = malloc(sizeof(unsigned int) * mat->rows);
	if (permutac == NULL)
		return 1;
	
	//allocate mamory for array of multiplicates
	matrix_multiplikator = malloc(sizeof(float) * mat->rows);
	if (matrix_multiplikator == NULL) {
		free(permutac);
		return 1;
	}

	//generate random positive multiplicators
	for (i = 0; i < mat->rows; i++) {
		matrix_multiplikator[i]= (float)(1 + 99.0 * rand() / (double)(RAND_MAX + 1));
		sum_matrix_multiplikator += matrix_multiplikator[i];
	}

	//normalize array with multiplikators sum{n[i]} = 1
	for (i = 0; i < mat->rows; i++) {
		matrix_multiplikator[i] /= sum_matrix_multiplikator;
	}
	

	//inicialaze array of permutation
	for (i = 0; i < mat->rows; i++) {
		permutac[i] = i;
	}

	//zeroing of matrix elemets
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->rows; j++) {
			ELEM(mat, i, j) = 0;
		}
	}
	
//	mix_array_of_permutation(permutac, mat->rows); // first pemutation
	
	//generate bistochastic matrix
	for (k = 0; k < mat->rows; k++) {
		mix_array_of_permutation(permutac, mat->rows);
		for (i = 0; i < mat->rows; i++) {
			for (j = 0; j < mat->rows; j++) {
				if (permutac[i] == j) 
					ELEM(mat, i, j) += matrix_multiplikator[k];
			}
		}
		//mix_array_of_permutation(permutac, mat->rows);
	}

	free(permutac);
	free(matrix_multiplikator);
	return 0;
}


int main() {
	MAT* mat;
	srand((unsigned int)time(NULL));

	mat = mat_create_with_type(4, 4);

	mat_create_random_bistochastic(mat);

	mat_print(mat);

	getchar();
	return 0;
}
