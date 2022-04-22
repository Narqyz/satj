#include "gaussSerial.h"
#include "math.h"
#include "stdio.h"

gaussSerial::gaussSerial(int size) {
	mSize = size;
	pSerialPivotIter = new int[size]; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
	pSerialPivotPos = new int[size];  // хранить порядок строк по итерации, нужен для обратного хода
	//Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	for (int i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1;
	}
}


int gaussSerial::resultCalculation(double** pMatrix, double* pVector, double* pResult) {
	// Тура жүріс, айнымалыларды Гаусс бойынша жою
	serialGaussianElimination(pMatrix, pVector);
	// Кері жүріс, айнымалыларды есептеу
	serialBackSubstitution(pMatrix, pVector, pResult);
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
int gaussSerial::serialGaussianElimination(double** pMatrix, double* pVector) {
	int Iter;
	// Гаусс итерациясының саны
	// жою
	int PivotRow;
	// Ағымдағы айналмалы жолдың саны
	for (Iter = 0; Iter < mSize; Iter++) {
		//жетекші жолды максимумы бойынша анықтау жолды табу
		PivotRow = findPivotRow(pMatrix, Iter);
		pSerialPivotPos[Iter] = PivotRow;
		pSerialPivotIter[PivotRow] = Iter;

		// Бағанның басқа элементтерін жою
		serialColumnElimination(pMatrix, pVector, PivotRow, Iter);
	}
	return 0;
}

 // Жиынтық жолды табуға арналған функция
int gaussSerial::findPivotRow(double** pMatrix, int Iter) {
	int PivotRow = -1; // Жиынтық жолдың индексі
	double MaxValue = 0; // Жиынтық элементтің мәні
	int i; // цикл айнымалысы
	// Ең көп элементті сақтайтын жолды таңдаңыз
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
	int RowIndex, Row;
	for (int i = mSize - 1; i >= 0; i--) {
		RowIndex = pSerialPivotPos[i];
		pResult[i] = pVector[RowIndex] / pMatrix[RowIndex][i];
		for (int j = 0; j < i; j++) {
			Row = pSerialPivotPos[j];
			pVector[j] -= pMatrix[Row][i] * pResult[i];
			pMatrix[Row][i] = 0;
		}
	}
	return 0;
}