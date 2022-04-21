#include "matrixHelpers.h"
#include "stdio.h"
#include "math.h"


/**
 * Функция проверяет результат:
 * умножает матрицу на вектор-решении и сравнивает с результатом
 */
int matrixHelpers::testSolvingResult(double** pMatrix, double* pVector, double* pResult, int Size) {
	/* Buffer for storing the vector, that is a result of multiplication
	of the linear system matrix by the vector of unknowns */
	double* pRightPartVector;
	// Flag, that shows wheather the right parts vectors are identical or not
	int equal = 0;
	double Accuracy = 1.e-2; // Comparison accuracy
	pRightPartVector = new double[Size];
	for (int i = 0; i < Size; i++) {
		pRightPartVector[i] = 0;
		for (int j = 0; j < Size; j++) {
			pRightPartVector[i] += pMatrix[i][j] * pResult[j];
		}
	}
	for (int i = 0; i < Size; i++) {
		if (fabs(pRightPartVector[i] - pVector[i]) > Accuracy) {
			equal = 1;
		}
	}
	if (equal == 1) {
		printf("Result is NOT correct. Check your code.\n");
	}
	else {
		//printf("The result of the algorithm is correct.\n");
	}

	delete[] pRightPartVector;


	return 0;
}