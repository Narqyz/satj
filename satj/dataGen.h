#pragma once

class dataGen {
public:
	static void randomDataInitialization(double** pMatrix, double* pVector, int Size);
	static void DiagonalDataInitialization(double** pMatrix, double* pVector, int Size);
	static void ThreeDiagonalDataInitialization(double** pMatrix, double* pVector, int Size);
private:

	dataGen(int size);

};

