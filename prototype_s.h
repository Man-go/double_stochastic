#define ELEM(M,r,c) (M->elem[(M->cols) * r + c])

typedef struct {
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;

MAT* mat_create_with_type(unsigned int rows, unsigned int cols);
MAT* mat_create_by_file(char* filename);
char mat_save(MAT* mat, char* filename);
void mat_destroy(MAT* mat);
void mat_unit(MAT* mat);
void mat_random(MAT* mat);
void mat_print(MAT* mat);
char mat_create_random_bistochastic(MAT* mat);
void mix_array_of_permutation(unsigned int* array_permutation, unsigned int dim);