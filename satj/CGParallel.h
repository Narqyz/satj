#pragma once
class CGParallel {
public:
	void resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size, int threads_count);
	int iterationsCount;
private:
	double diff(double *vector1, double* vector2, int Size);
};

