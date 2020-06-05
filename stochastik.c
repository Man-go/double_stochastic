#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define n 3 //pocet_riadkov rows
#define m 3 //pocet_stlpcov cols
#define Dim 4
#define ELEM(M,r,c) (M->elem[(M->cols) * r + c])

typedef struct {
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

/*prototyps*/
void mat_generate_with_type(unsigned int rows, unsigned int cols); //Yes
MAT* mat_create_with_type(unsigned int rows, unsigned int cols); //Yes
MAT* mat_create_by_file(char* filename); //Yes
char mat_save(MAT* mat, char* filename); //Yes
void mat_destroy(MAT* mat); // Yes
void mat_unit(MAT* mat);  //Yes
void mat_random(MAT* mat); //Yes
void mat_print(MAT* mat); //Yes
char mat_create_random_bistochastic(MAT* mat); //Yes
void mat_print_from_file(char* filename); //Yes
void mat_generate_random_bistochastic(char* filename); //Yes
void mix_array_of_permutation(unsigned int array_permutation, unsigned int dim); //Yes

//generuje maticu nejakou maticu a ulozi do souboru matica.bin
void mat_generate_with_type(unsigned int rows, unsigned int cols) {
	int i;
	float elem_matici;
	FILE* fw = NULL;
	if ((fw = fopen("matica.bin", "wb")) == NULL) {
		printf("Subor matica.bin sa nepodarilo otvorit!\n");
		return;
	}
	fwrite("M", sizeof(char), 1, fw);
	fwrite("1", sizeof(char), 1, fw);
	fwrite(&rows, sizeof(unsigned int), 1, fw);
	fwrite(&cols, sizeof(unsigned int), 1, fw);
	for (i = 0; i < n * m; i++) {
		elem_matici = -10 + 20.0 * rand() / (double)(RAND_MAX + 1);
		fwrite(&elem_matici, sizeof(float), 1, fw);
	}
	fclose(fw);
}

//allocate and matrix nxm
MAT* mat_create_with_type(unsigned int rows_mat, unsigned int cols_mat) {
	MAT* M = malloc(sizeof(MAT));
	M->rows = rows_mat;
	M->cols = cols_mat;
	M->elem = (float*)malloc(sizeof(float) * rows_mat * cols_mat);
	if (M->elem == NULL)
		return 1;

	return M;
}

//Save matrix from memory to file w noticed name
char mat_save(MAT* mat, char* filename) {
	int i;
	float elem_matici;
	FILE* fw = NULL;
	if ((fw = fopen(filename, "wb")) == NULL) {
		printf("Subor %s sa nepodarilo otvorit!\n", filename);
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

//print on screen matrix from file
void mat_print_from_file(char* filename) {
	FILE* fr = NULL;
	unsigned int rows, cols, i;
	float temp_elem;
	char ch[2];

	if ((fr = fopen(filename, "rb")) == NULL) {
		printf("Subor %s sa nepodarilo otvorit!\n", filename);
		return 1;
	}

	fread(&ch[0], sizeof(char), 1, fr);
	fread(&ch[1], sizeof(char), 1, fr);
	fread(&rows, sizeof(int), 1, fr);
	fread(&cols, sizeof(int), 1, fr);
	printf("%c %c\n%d %d\n", ch[0], ch[1], rows, cols);

	for (i = 0; i < rows * cols; i++) {
		fread(&temp_elem, sizeof(float), 1, fr);
		printf("%f\t", temp_elem);
		if (i % rows == rows - 1)
			printf("\n");
	}

	fclose(fr);
}


//vypise maticu mat na obrazovku z memory print on screen matrix from memory
void mat_print(MAT* mat) {
	int i;

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
	int i;

	for (i = 0; i < mat->rows * mat->cols; i++) {
		mat->elem [i] = -1 + 2.0 * rand() / (double)(RAND_MAX + 1);
	}
}


//inicializuje hustu maticu mat tak, aby vsetky mimodiagonalne elementy boli nulove a vsetky diagonalne elementy jednotkove
void mat_unit(MAT* mat) {
	int i,j;

	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			if (i == j)
				ELEM(mat, i, j) = 1;
			else
				ELEM(mat, i, j) = 0;
		}
	}
}


//change order of permutation 0312 -> 3120
void mix_array_of_permutation(unsigned int array_permutation[], unsigned int dim) {
	int i;
	unsigned int temp_element_of_permutation;

	temp_element_of_permutation = array_permutation[0];
	for (i = 0; i < dim-1; i++) {
		array_permutation[i] = array_permutation[i + 1];
	}
	array_permutation[dim-1] = temp_element_of_permutation;
}


//generuje randomnu bistochastic matrix
void mat_generate_random_bistochastic(char* filename) {
	int i, temp_swap, j, k;
	unsigned int permutac[Dim];

	float random_matrix_multiplikator;
	MAT* p_Bistocastic_M;

	p_Bistocastic_M = mat_create_with_type(Dim, Dim);

	for (i = 0; i < Dim; i++) {
		permutac[i] = i;
	}

	for (i = Dim-1; i>=1; i--) {
		int j = rand() % (i+1);
		temp_swap = permutac[j];
		permutac[j] = permutac[i];
		permutac[i] = temp_swap;
	}

	for (k = 0; k < Dim; k++) {
		random_matrix_multiplikator = 1 + 99.0 * rand() / (double)(RAND_MAX + 1);
		for (i = 0; i < Dim; i++) {
			for (j = 0; j < Dim; j++) {
				if (permutac[i] == j)
					ELEM(p_Bistocastic_M, i, j) = random_matrix_multiplikator;
			}
		}
		mix_array_of_permutation(permutac, Dim);
	}

	mat_save(p_Bistocastic_M, filename);

	mat_print(p_Bistocastic_M);
	printf("\n===========\n");

	mat_create_random_bistochastic(p_Bistocastic_M);

	mat_destroy(p_Bistocastic_M);
}

//upravi matricu na bistochasticku pokud je to mozne
char mat_create_random_bistochastic(MAT* mat) {
	int i, j;
	float temp_sum = 0, sum_rows = 0, sum_cols = 0;

	if (mat->rows != mat->cols) {
		printf("Tenta matica nie bistochasticka");
		return 1;
	}
	
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			if (ELEM(mat, i, j) < 0) {
				printf("Tenta matica nie bistochasticka");
				return 1;
			}
		}
	}

	for (j = 0; j < mat->cols; j++) {
		temp_sum += ELEM(mat, 0, j);
	}

	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			sum_rows += ELEM(mat, i, j);
			sum_cols += ELEM(mat, j, i);
		}
		if (sum_rows != temp_sum || sum_cols != temp_sum) {
			printf("Tenta matica nie bistochasticka");
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

	mat_print(mat);
	
	char bistoch_file_name[40] = "bistochastic_reduced_matrix.bin";
	char* p_bistoch_file_name;
	p_bistoch_file_name = &bistoch_file_name;

	mat_save(mat, bistoch_file_name);

	printf("Ano! Matica Bistochasticka\n\n");
	return 0;	
}

//read matrix from file into memory
MAT* mat_create_by_file(char* filename) {
	FILE* fr = NULL;
	unsigned int row_m, cols_m, i;
	MAT* p_M;
	float temp_elem;

	if ((fr = fopen(filename, "rb")) == NULL) {
		printf("Subor %s sa nepodarilo otvorit!\n", filename);
		return;
	}

	char ch[2];

	fread(&ch[0], sizeof(char), 1, fr);
	fread(&ch[1], sizeof(char), 1, fr);
	fread(&row_m, sizeof(unsigned int), 1, fr);
	fread(&cols_m, sizeof(unsigned int), 1, fr);

	p_M = mat_create_with_type(row_m, cols_m);

	for (i = 0; i < row_m * cols_m; i++) {
		fread(&temp_elem, sizeof(float), 1, fr);
		p_M->elem [i] = temp_elem;
	}

	mat_print_from_file(filename);

	fclose(fr);
}





int main() {
	srand((unsigned int)time(NULL));

	char filename_bistochastic[50] = "matica_bistochastic.bin";
	char* p_filename_bistochastic;
	p_filename_bistochastic = &filename_bistochastic;

	mat_generate_random_bistochastic(p_filename_bistochastic);

	getchar();
	return 0;
}