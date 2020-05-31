#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define n 3 //pocet_riadkov rows
#define m 3 //pocet_stlpcov cols
#define N 15
#define ELEM(M,r,c) (M.elm[(M.cols)*r+c])

typedef struct {
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

/*prototyps*/
void mat_generate_with_type(unsigned int rows, unsigned int cols);
MAT* mat_create_with_type(unsigned int rows, unsigned int cols);
MAT* mat_create_by_file(char* filename);
char mat_save(MAT* mat, char* filename);
void mat_destroy(MAT* mat);
void mat_unit(MAT* mat);
void mat_random(MAT* mat);
void mat_print(MAT* mat);
char mat_create_random_bistochastic(MAT* mat);

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


MAT* mat_create_with_type(unsigned int rows_mat, unsigned int cols_mat) {
	MAT M; 
	M.rows = rows_mat;
	M.cols = cols_mat;
	M.elem = (float*)malloc(sizeof(float) * rows_mat * cols_mat);
	if (M.elem == NULL)
		return 1;
	
	return &M;
}


MAT* mat_create_by_file(char* filename) {
	FILE* fr = NULL;
	unsigned int row_m, cols_m, i;
	MAT* p_M;
	float temp_elem;
	float* p_temp_elem;

	if ((fr = fopen(filename, "rb")) == NULL) {
		printf("Subor matica.bin sa nepodarilo otvorit!\n");
		return;
	}

	char ch[2];

	fread(&ch[0], sizeof(char), 1, fr);
	fread(&ch[1], sizeof(char), 1, fr);
	fread(&row_m, sizeof(unsigned int), 1, fr);
	fread(&cols_m, sizeof(unsigned int), 1, fr);

	p_M = mat_create_with_type(row_m, cols_m);

	p_temp_elem = p_M->elem;
	for (i = 0; i < row_m * cols_m; i++) {
		fread(&temp_elem, sizeof(float), 1, fr);
		p_temp_elem[i] = temp_elem;
	}

	int j;

	p_M->elem = p_temp_elem;

	for (i = 0; i < row_m* cols_m; i++) {
			printf("%f ", p_temp_elem[i]);
		}

	

	//printf("%c %c %d %d\n", ch[0], ch[1], row_m, cols_m);

	
	fclose(fr);
}


int main() {
	srand((unsigned int)time(NULL));

	char filename[20] = "matica.bin";
	char* p_filename;

	p_filename = &filename;

	//mat_generate_with_type(n, m);
	
	mat_create_by_file(p_filename);

	//mat_create_with_type(n, m);

	getchar();
	return 0;
}
