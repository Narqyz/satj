#pragma once
class gaussSerial{
public:
	gaussSerial(int size);
	int resultCalculation(double** pMatrix, double* pVector, double* pResult);

private:
	int mSize;
	int* pSerialPivotPos; // Итерацияларда таңдалған айналмалы жолдардың саны
	int* pSerialPivotIter; // Жолдар бұрылу болатын итерациялар

	int findPivotRow(double** pMatrix, int Iter);
	void serialGaussianElimination(double** pMatrix, double* pVector);
	void serialBackSubstitution(double** pMatrix, double* pVector, double* pResult);
	void serialColumnElimination(double** pMatrix, double* pVector, int Pivot, int Iter);

};

