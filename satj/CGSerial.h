#pragma once
class CGSerial {
public:
	void resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size);
	int iterationsCount;
private:
	double diff(double *vector1, double* vector2, int Size);

};

