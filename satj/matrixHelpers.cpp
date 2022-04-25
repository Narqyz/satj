#include "matrixHelpers.h"
#include "stdio.h"
#include "math.h"
#include <vcruntime_string.h>
#include <omp.h>


/**
 * Функция проверяет результат:
 * умножает матрицу на вектор-решении и сравнивает с результатом
 */
int matrixHelpers::testSolvingResult(double** pMatrix, double* pVector, double* pResult, int Size) {
	/* Buffer for storing the vector, that is a result of multiplication
	of the linear system matrix by the vector of unknowns */
	double* pRightPartVector;
	// Flag, that shows wheather the right parts vectors are identical or not
	int equal = 0, i, j;
	double Accuracy = 0.01f; // Comparison accuracy
	pRightPartVector = new double[Size];

	for (i = 0; i < Size; i++) {
		pRightPartVector[i] = 0;
		for (j = 0; j < Size; j++) {
			pRightPartVector[i] += pMatrix[i][j] * pResult[j];
		}
	}

#pragma omp parallel for reduction(+:equal)
	for (i = 0; i < Size; i++) {
		if (fabs(pRightPartVector[i] - pVector[i]) > Accuracy) {
			equal++;
		}
	}
	if (equal > 1) {
		printf(" Wrong.");
	}
	else {
		//printf(" Сorrect.");
	}
	delete[] pRightPartVector;
	return 0;
}
bool matrixHelpers::checkSymmetrical(double** pMatrix, int Size) {
	for (int i = 0; i < Size; i++) {
		for (int j = i; j < Size; j++) {
			if (pMatrix[i][j] != pMatrix[j][i]) return false;
		}
	}
	return true;
}
bool matrixHelpers::checkPositiveDefinite(double** pMatrix, int Size)
{
	return true;
}
void matrixHelpers::printVector(double* matrix, int size) {

	for (int i = 0; i < size; i++) {
		printf("%.4f ", matrix[i]);
	}
	printf("\n");
}

void matrixHelpers::printMatrix(double** matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%.4f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}