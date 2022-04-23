#pragma once
class matrixHelpers {
public:


	static void printVector(double* matrix, int size);
	static void printMatrix(double** matrix, int size);
	static int testSolvingResult(double** pMatrix, double* pVector, double* pResult, int Size);
	static bool checkSymmetrical(double** pMatrix, int Size);

};

