#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
//#include <io.h>
//#include <unistd.h>



#define ELEM(M,r,c) (M->elem[(M->cols) * r + c])

typedef struct {
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

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

//Save matrix from memory to file w noticed name
char mat_save(MAT* mat, char* filename) {
	unsigned int i;
	float elem_matici = 0;
	FILE* fw = NULL;
	if ((fw = fopen(filename, "wb")) == NULL) {
		return 1;
	}

	fwrite("M", sizeof(char), 1, fw);
	fwrite("1", sizeof(char), 1, fw);
	fwrite(&mat->rows, sizeof(unsigned int), 1, fw);
	fwrite(&mat->cols, sizeof(unsigned int), 1, fw);
	
	for (i = 0; i < mat->rows * mat->cols; i++) {
		fwrite(&mat->elem [i], sizeof(float), 1, fw);
	}
	fclose(fw);

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


//upravi matricu na bistochasticku pokud je to mozne
char mat_create_random_bistochastic(MAT* mat) {
	unsigned int i, j;
	float temp_sum = 0, sum_rows = 0, sum_cols = 0;

	if (mat->rows != mat->cols)
		return 1;
	
	
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			if (ELEM(mat, i, j) < 0) {
				return 1;
			}
		}
	}

	for (j = 0; j < mat->cols; j++) 
		temp_sum += ELEM(mat, 0, j);
	

	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			sum_rows += ELEM(mat, i, j);
			sum_cols += ELEM(mat, j, i);
		}
		if (sum_rows != temp_sum || sum_cols != temp_sum) {
			return 1;
		}
		sum_rows = 0;
		sum_cols = 0;
	}

	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			ELEM(mat, i, j) /= temp_sum;
		}
	}
	return 0;	
}

//read matrix from file into memory
MAT* mat_create_by_file(char* filename) {
	//FILE* fr = NULL;
	int fr;
	unsigned int rows_m = 0, cols_m = 0, i = 0;
	MAT* p_M;
	float temp_elem = 0;

	if ((fr = open(filename, O_RDONLY)) == -1) {
		return NULL;
	}
	//if ((fr = open(filename, "rb")) == -1) {
	//	return NULL;
	//}

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

int main() {


	getchar();
	return 0;
}