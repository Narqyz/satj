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
		pVector[i] = (double) rand() / double(Size);
		for (j = 0; j < Size; j++) {
			double tmp = (double) rand() / double(Size);
			if (i == j) {
				if (tmp < 0)
					pMatrix[i][j] = (-1) * tmp;
				else
					pMatrix[i][j] = tmp;
			}
			else {
				pMatrix[i][j] = pMatrix[j][i] = tmp;
			}
		}
	}
	return 0;
}
/**
 * Матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
int dataGen::DiagonalDataInitialization(double** pMatrix, double* pVector, int Size) {
	int i, j; // Цикл айнымалылары
	srand(unsigned(clock()));
	for (i = 0; i < Size; i++) {
		pVector[i] = (double) rand() / double(Size);
		for (j = 0; j < Size; j++) {
			if (i == j) {
				pMatrix[i][j] = (double) rand() / double(Size);
			}
			else {
				pMatrix[i][j] = 0;
			}
		}
	}
	return 0;
}


