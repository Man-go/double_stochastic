#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define n 3 //pocet_riadkov rows
#define m 3 //pocet_stlpcov cols
#define N 15
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
char mat_create_random_bistochastic(MAT* mat);
void mat_print_from_file(char* filename); //Yes

//generuje maticu
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


	/*char ch, ch1;
	int number, number2,j;
	float fff;
	FILE* input = NULL;
	if ((input = fopen("matica.bin", "rb")) == NULL) {
		printf("Subor matica.bin sa nepodarilo otvorit!\n");
		return;
	}
	fread(&ch, sizeof(char), 1, input);
	fread(&ch1, sizeof(char), 1, input);
	fread(&number, sizeof(int), 1, input);
	fread(&number2, sizeof(int), 1, input);
	printf("%c %c %d %d\n", ch, ch1, number, number2);
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			fread(&fff, sizeof(float), 1, input);
			printf("%f\t", fff);
		}
		printf("\n");
	}
	fclose(input);
	*/
}

//allocate and matrix nxm
MAT* mat_create_with_type(unsigned int rows_mat, unsigned int cols_mat) {
	//MAT M;
	MAT* M = malloc(sizeof(unsigned int)*2 + 4);
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


//vypise maticu mat na obrazovku z memory
//print on screen matrix from memory
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


//inicializuje hustu maticu mat tak, aby vsetky mimodiagonalne
//elementy boli nulove a vsetky diagonalne elementy jednotkove
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



char mat_create_random_bistochastic(MAT* mat) {

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

	mat_unit(p_M);
}





int main() {
	srand((unsigned int)time(NULL));

	char filename[20] = "matica.bin";
	char* p_filename;

	p_filename = &filename;

	//mat_generate_with_type(n, m);

	mat_create_by_file(p_filename);

	getchar();
	return 0;
}