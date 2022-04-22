#include "gaussSerial.h"
#include "math.h"
#include "stdio.h"

gaussSerial::gaussSerial(int size) {
	mSize = size;
	pSerialPivotIter = new int[size]; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
	pSerialPivotPos = new int[size];  // хранить порядок строк по итерации, нужен для обратного хода	
	for (int i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1; //Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	}
}


int gaussSerial::resultCalculation(double** pMatrix, double* pVector, double* pResult) {
	serialGaussianElimination(pMatrix, pVector);	// Тура жүріс, айнымалыларды Гаусс бойынша жою
	serialBackSubstitution(pMatrix, pVector, pResult); // Кері жүріс, айнымалыларды есептеу	
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
int gaussSerial::serialGaussianElimination(double** pMatrix, double* pVector) {
	int Iter; // Гаусс итерациясының саны	
	int PivotRow; // жою
	// Ағымдағы айналмалы жолдың саны
	for (Iter = 0; Iter < mSize; Iter++) {
		PivotRow = findPivotRow(pMatrix, Iter); //жетекші жолды максимумы бойынша анықтау жолды табу
		pSerialPivotPos[Iter] = PivotRow; // хранить порядок строк по итерации, нужен для обратного хода
		pSerialPivotIter[PivotRow] = Iter; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
		serialColumnElimination(pMatrix, pVector, PivotRow, Iter);
	}
	return 0;
}

// Iter бағаны үшін макс элементті жолды таңдау жолды табу
int gaussSerial::findPivotRow(double** pMatrix, int Iter) {
	double MaxValue = 0; // бағандағы макс мәні
	int PivotRow = -1; // бағандағы макс мән жолдың индексі
	int i; // цикл айнымалысы
	// жолдар бойынша макс іздеу және ол жол алдын қолданылмаған болу керек
	for (i = 0; i < mSize; i++) {
		if ((pSerialPivotIter[i] == -1) && (fabs(pMatrix[i][Iter]) > MaxValue)) {
			PivotRow = i;
			MaxValue = fabs(pMatrix[i][Iter]);
		}
	}
	return PivotRow;
}

// Бағанның басқа элементтерін жою
int gaussSerial::serialColumnElimination(double** pMatrix, double* pVector, int Pivot, int Iter) {
	double PivotValue, PivotFactor;
	PivotValue = pMatrix[Pivot][Iter];
	for (int i = 0; i < mSize; i++) {
		if (pSerialPivotIter[i] == -1) {
			PivotFactor = pMatrix[i][Iter] / PivotValue;
			for (int j = Iter; j < mSize; j++) {
				pMatrix[i][j] -= PivotFactor * pMatrix[Pivot][j];
			}
			pVector[i] -= PivotFactor * pVector[Pivot];
		}
	}
	return 0;
}

// Кері жүріс
int gaussSerial::serialBackSubstitution(double** pMatrix, double* pVector, double* pResult) {
	int RowIndex;
	for (int i = mSize - 1; i >= 0; i--) {
		RowIndex = pSerialPivotPos[i];
		pResult[i] = pVector[RowIndex] / pMatrix[RowIndex][i];
		for (int j = i + 1; j < mSize; j++) {
			pResult[i] -= pMatrix[RowIndex][j] * pResult[j] / pMatrix[RowIndex][i];
		}
	}
	return 0;
}