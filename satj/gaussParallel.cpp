#include "gaussParallel.h"
#include "math.h"
#include "stdio.h"
#include <omp.h>
#include "types.h"
#include <iostream>

//конструктор - условие для доступа к классу
gaussParallel::gaussParallel(int size, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 	
	mSize = size;
	pSerialPivotIter = new int[size]; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
	pSerialPivotPos = new int[size]; // хранить порядок строк по итерации, нужен для обратного хода		
#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1; // Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	}
}

int gaussParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult) {	 
	gaussianElimination(pMatrix, pVector); // Тура жүріс, айнымалыларды Гаусс бойынша жою	
	backSubstitution(pMatrix, pVector, pResult); // Кері жүріс, айнымалыларды есептеу
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
void gaussParallel::gaussianElimination(double** pMatrix, double* pVector) {
	for (int Iter = 0; Iter < mSize; Iter++) {
		int PivotRow = findPivotRow(pMatrix, Iter); // бағандағы макс табу
		pSerialPivotPos[Iter] = PivotRow; // хранить порядок строк по итерации, нужен для обратного хода
		pSerialPivotIter[PivotRow] = Iter; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода		
		columnElimination(pMatrix, pVector, PivotRow, Iter); // Бағанның басқа элементтерін жою
	}
}

// Iter бағаны үшін макс элементті жолды таңдау жолды табу
int gaussParallel::findPivotRow(double** pMatrix, int Iter) {
	double MaxValue = 0; // бағандағы макс элементтің мәні
	int PivotRow = -1; // бағандағы макс элементті жолдың индексі
	int i;
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
		}
	}
	return PivotRow;
}

void gaussParallel::columnElimination(double** pMatrix, double* pVector, int Pivot, int Iter) {
	 double PivotValue = pMatrix[Pivot][Iter];
#pragma omp parallel for private (PivotFactor) schedule(dynamic, 1) 
	for (int i = 0; i < mSize; i++) {  
		if (pSerialPivotIter[i] == -1) {
			double PivotFactor = pMatrix[i][Iter] / PivotValue;
			for (int j = Iter; j < mSize; j++) {
				pMatrix[i][j] -= PivotFactor * pMatrix[Pivot][j];
			}
			pVector[i] -= PivotFactor * pVector[Pivot];
		}
	}
}

// Кері жүріс
void gaussParallel::backSubstitution(double** pMatrix, double* pVector, double* pResult) {
	int RowIndex, Row;
	for (int i = mSize - 1; i >= 0; i--) {
		RowIndex = pSerialPivotPos[i];
		double tmp = pVector[RowIndex] / pMatrix[RowIndex][i];
#pragma omp parallel for reduction (-:tmp)
		for (int j = i + 1; j < mSize; j++) {
			tmp -= pMatrix[RowIndex][j] * pResult[j] / pMatrix[RowIndex][i];
		}
		pResult[i] = tmp;
	}
}