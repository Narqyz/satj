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
		pVector[i] = (double)rand();
		for (j = 0; j < Size; j++) {
			double tmp = (double)rand();
			if (tmp < 0) tmp *= -1;
			pMatrix[i][j] = pMatrix[j][i] = tmp;
		}
	}
	return 0;
}


