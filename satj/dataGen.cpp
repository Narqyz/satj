#include "dataGen.h"
#include "ctime"
#include "cstdlib"

/**
 * Матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
void dataGen::randomDataInitialization(double** pMatrix, double* pVector, int Size) {
	int i, j; // Цикл айнымалылары
	srand(unsigned(clock()));
	for (i = 0; i < Size; i++) {
		pVector[i] = (double) rand() / double(Size);
		for (j = 0; j < Size; j++) {
				pMatrix[i][j] = pMatrix[j][i] = (double)rand() / double(Size);
		}
	}
}
/**
 * диагональ матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
void dataGen::DiagonalDataInitialization(double** pMatrix, double* pVector, int Size) {
	srand(unsigned(clock()));
	for (int i = 0; i < Size; i++) {
		pVector[i] = (double)rand() / double(Size);
		for (int j = 0; j < Size; j++) {
			if (i == j) {
				pMatrix[i][j] = (double) rand() / double(Size);
			}
			else {
				pMatrix[i][j] = 0;
			}
		}
	}
}
/**
 * 3 - диагональді матрица мен вектор элементтерін кездейсоқ инициализациялау функциясы
 */
void dataGen::ThreeDiagonalDataInitialization(double** pMatrix, double* pVector, int Size) {
	srand(unsigned(clock()));
	for (int i = 0; i < Size; i++) {
		pVector[i] = (double)rand() / double(Size);
		for (int j = 0; j < Size; j++) {
			if (fabs(j - i) < 2) {
				pMatrix[i][j] = pMatrix[j][i] = (double)rand() / double(Size);
			}
			else {
				pMatrix[i][j] = 0;
			}
		}
	}
}


