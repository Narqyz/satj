#include "dataGen.h"
#include "ctime"
#include "cstdlib"

/**
 * Матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
int dataGen::randomDataInitialization(double** pMatrix, double* pVector, int Size) {
	int i, j; // Цикл айнымалылары

	srand(unsigned(clock()));
	for (i = 0; i < Size; i++) {
		pVector[i] = (double)rand() / RAND_MAX;
		for (j = 0; j < Size; j++) {
			pMatrix[i][j] = pMatrix[j][i] = (double)rand() / RAND_MAX;
		}
	}
	return 0;
}


