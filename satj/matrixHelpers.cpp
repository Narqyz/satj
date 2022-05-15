#include "matrixHelpers.h"
#include "stdio.h"
#include "math.h"
#include <vcruntime_string.h>
#include <omp.h>


/**
 * Функция проверяет результат:
 * умножает матрицу на вектор-решении и сравнивает с результатом
 */
void matrixHelpers::testSolvingResult(double** pMatrix, double* pVector, double* pResult, int Size) {
	double* pRightPartVector;
	int equal = 0, i, j;
	double Accuracy = 0.01f;
	pRightPartVector = new double[Size];
	for (i = 0; i < Size; i++) {
		double tmp = 0;
#pragma omp parallel for reduction(+:tmp)
		for (j = 0; j < Size; j++) {
			tmp += pMatrix[i][j] * pResult[j];
		}
		pRightPartVector[i] = tmp;
	}
#pragma omp parallel for reduction(+:equal)
	for (i = 0; i < Size; i++) {
		if (fabs(pRightPartVector[i] - pVector[i]) > Accuracy) {
			equal++;
		}
	}
	if (equal > 0) {
		printf(" Wrong.");
	}
	else {
		//printf(" Сorrect.");
	}
	delete[] pRightPartVector;
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

void matrixHelpers::setDefault(double** originalA, double* originalB, int size, double** A,double* B, double* X) {
#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		B[i] = originalB[i];
		X[i] = 0;
		for (int j = 0; j < size; j++)
			A[i][j] = originalA[i][j];
	}
}