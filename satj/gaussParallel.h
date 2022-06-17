#pragma once
class gaussParallel{
public:
	gaussParallel(int size, int threads_count);
	int resultCalculation(double** pMatrix, double* pVector, double* pResult);
private:
	int mSize;
	int* pSerialPivotPos; // Итерацияларда таңдалған айналмалы жолдардың саны
	int* pSerialPivotIter; // Жолдар бұрылу болатын итерациялар

	int findPivotRow(double** pMatrix, int Iter);
	void gaussianElimination(double** pMatrix, double* pVector);
	void backSubstitution(double** pMatrix, double* pVector, double* pResult);
	void columnElimination(double** pMatrix, double* pVector, int Pivot, int Iter);
};

