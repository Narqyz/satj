#pragma once
class gaussParallel{
public:
	gaussParallel(int size);
	int resultCalculation(double** pMatrix, double* pVector, double* pResult, int threads_count);
private:
	int mSize;
	int* pSerialPivotPos; // Итерацияларда таңдалған айналмалы жолдардың саны
	int* pSerialPivotIter; // Жолдар бұрылу болатын итерациялар

	int findPivotRow(double** pMatrix, int Iter);
	int gaussianElimination(double** pMatrix, double* pVector);
	int backSubstitution(double** pMatrix, double* pVector, double* pResult);
	int columnElimination(double** pMatrix, double* pVector, int Pivot, int Iter);
};

