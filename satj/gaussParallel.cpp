#include "gaussParallel.h"
#include "math.h"
#include "stdio.h"
#include "omp.h"
#include "types.h"


gaussParallel::gaussParallel(int size) {
	mSize = size;

	pSerialPivotIter = new int[size];
	pSerialPivotPos = new int[size];
	//Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	for (int i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1;
	}
}

int gaussParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 
	// Тура жүріс, айнымалыларды Гаусс бойынша жою
	gaussianElimination(pMatrix, pVector);
	// Кері жүріс, айнымалыларды есептеу
	backSubstitution(pMatrix, pVector, pResult);
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
int gaussParallel::gaussianElimination(double** pMatrix, double* pVector) {
	int Iter;
	// Гаусс итерациясының саны
	// жою
	int PivotRow;
	// Ағымдағы айналмалы жолдың саны
	for (Iter = 0; Iter < mSize; Iter++) {
		//Айналмалы жолды табу
		PivotRow = findPivotRow(pMatrix, Iter);
		pSerialPivotPos[Iter] = PivotRow;
		pSerialPivotIter[PivotRow] = Iter;
		// Бағанның басқа элементтерін жою
		columnElimination(pMatrix, pVector, PivotRow, Iter);
	}
	return 0;
}

// Жиынтық жолды табуға арналған функция
int gaussParallel::findPivotRow(double** pMatrix, int Iter) {
	int PivotRow = -1; // Жиынтық жолдың индексі
	double MaxValue = 0; // Жиынтық элементтің мәні
	int i; // цикл айнымалысы
	// Ең көп элементті сақтайтын жолды таңдаңыз
#pragma omp parallel
	{
		TThreadPivotRow ThreadPivotRow;
		ThreadPivotRow.MaxValue = 0;
		ThreadPivotRow.PivotRow = -1;

#pragma omp for
		for (i = 0; i < mSize; i++) {
			if ((pSerialPivotIter[i] == -1) && (fabs(pMatrix[i][Iter]) > ThreadPivotRow.MaxValue)) {
				ThreadPivotRow.PivotRow = i;
				ThreadPivotRow.MaxValue = fabs(pMatrix[i][Iter]);
			}
		}

#pragma omp critical
		{
			if (ThreadPivotRow.MaxValue > MaxValue) {
				MaxValue = ThreadPivotRow.MaxValue;
				PivotRow = ThreadPivotRow.PivotRow;
			}
		} // pragma omp critical
	}// pragma omp parallel

	return PivotRow;
}


// Бағанның басқа элементтерін жою
int gaussParallel::columnElimination(double** pMatrix, double* pVector, int Pivot, int Iter) {
	double PivotValue, PivotFactor;
	PivotValue = pMatrix[Pivot][Iter];
#pragma omp parallel for private (PivotFactor) schedule(dynamic,1)
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
int gaussParallel::backSubstitution(double** pMatrix, double* pVector, double* pResult) {
	int RowIndex, Row;
	for (int i = mSize - 1; i >= 0; i--) {
		RowIndex = pSerialPivotPos[i];
		pResult[i] = pVector[RowIndex] / pMatrix[RowIndex][i];
#pragma omp parallel for private (Row)
		for (int j = 0; j < i; j++) {
			Row = pSerialPivotPos[j];
			pVector[j] -= pMatrix[Row][i] * pResult[i];
			pMatrix[Row][i] = 0;
		}
	}

	return 0;
}