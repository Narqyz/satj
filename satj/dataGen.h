#pragma once

class dataGen {
public:
	static int randomDataInitialization(double** pMatrix, double* pVector, int Size);
	static int DiagonalDataInitialization(double** pMatrix, double* pVector, int Size);
private:

	dataGen(int size);

};

