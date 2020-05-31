#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define n 7 //pocet_riadkov
#define m 7 //pocet_stlpcov
#define N 15
#define ELEM(M,r,c) (M.elm[(M.cols)*r+c])

typedef struct {
	unsigned int rows;
	unsigned int cols;
	float* elem;
}MAT;


int main() {
	srand((unsigned int)time(NULL));

	printf("Hellow Fucking World");

	getchar();
	return 0;
}
