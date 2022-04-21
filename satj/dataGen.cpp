#include "dataGen.h"
#include "ctime"
#include "cstdlib"

/**
 * Матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
int dataGen::randomDataInitialization(double** pMatrix, double* pVector, int Size) {
	int i, j; // Цикл айнымалылары
	for (i = 0; i < Size; i++) {
		pVector[i] = i + 1;
		for (j = 0; j < Size; j++) {
			if (j == i) {
				pMatrix[i][j] = (double)rand() / RAND_MAX * 1000;
			}
			else {
				pMatrix[i][j] = 0;//(double)rand() / RAND_MAX * 1000;
			}
		}
	}
	return 0;
}

