#include "gaussParallel.h"
#include "math.h"
#include "stdio.h"
#include "omp.h"
#include "types.h"

//конструктор - условие для доступа к классу
gaussParallel::gaussParallel(int size) {
	mSize = size;
	pSerialPivotIter = new int[size]; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
	pSerialPivotPos = new int[size]; // хранить порядок строк по итерации, нужен для обратного хода		
#pragma omp parallel for
	for (size_t i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1; // Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	}
}

int gaussParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 	
	gaussianElimination(pMatrix, pVector); // Тура жүріс, айнымалыларды Гаусс бойынша жою	
	backSubstitution(pMatrix, pVector, pResult); // Кері жүріс, айнымалыларды есептеу
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
int gaussParallel::gaussianElimination(double** pMatrix, double* pVector) {
	for (size_t Iter = 0; Iter < mSize; Iter++) {
		int PivotRow = findPivotRow(pMatrix, Iter); // бағандағы макс табу
		pSerialPivotPos[Iter] = PivotRow; // хранить порядок строк по итерации, нужен для обратного хода
		pSerialPivotIter[PivotRow] = Iter; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода		
		columnElimination(pMatrix, pVector, PivotRow, Iter); // Бағанның басқа элементтерін жою
	}
	return 0;
}

// Iter бағаны үшін макс элементті жолды таңдау жолды табу
int gaussParallel::findPivotRow(double** pMatrix, int Iter) {
	double MaxValue = 0; // бағандағы макс элементтің мәні
	int PivotRow = -1; // бағандағы макс элементті жолдың индексі
#pragma omp parallel
	{
		TThreadPivotRow ThreadPivotRow;
		ThreadPivotRow.MaxValue = 0; //задаем макс переменные каждому потоку
		ThreadPivotRow.PivotRow = -1; //задаем ид строки со макс значением каждому потоку

	// жолдар бойынша макс іздеу және ол жол алдын қолданылмаған болу керек
#pragma omp for
		for (size_t i = 0; i < mSize; i++) {
			if ((pSerialPivotIter[i] == -1) && (fabs(pMatrix[i][Iter]) > ThreadPivotRow.MaxValue)) {
				ThreadPivotRow.PivotRow = i;
				ThreadPivotRow.MaxValue = fabs(pMatrix[i][Iter]);
			}
		}
#pragma omp critical // әр поток үшін 1 рет қана жүретінін көрсету ал қалған потоктар кезекте тұрады
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
	double PivotValue = pMatrix[Pivot][Iter];
#pragma omp parallel for private (PivotFactor) schedule(dynamic) // әр потокта өзінің PivotFactor бар, әр поток бос болса 1-ақ блок жұмыс істеуге алады. ол нагрузканы бөлуге көмектеседі
	for (size_t i = 0; i < mSize; i++) {  // ti1:i=0; ti2:i=1; --> ti2:i=2;-->ti2:i=3;-->ti1
		if (pSerialPivotIter[i] == -1) {
			double PivotFactor = pMatrix[i][Iter] / PivotValue;
			for (size_t j = Iter; j < mSize; j++) {
				pMatrix[i][j] -= PivotFactor * pMatrix[Pivot][j];
			}
			pVector[i] -= PivotFactor * pVector[Pivot];
		}
	}
	return 0;
}

// Кері жүріс
int gaussParallel::backSubstitution(double** pMatrix, double* pVector, double* pResult) {

#pragma omp parallel for private(RowIndex)
	for (int i = mSize - 1; i >= 0; i--) {
		int RowIndex = pSerialPivotPos[i];
		pResult[i] = pVector[RowIndex] / pMatrix[RowIndex][i];
		for (size_t j = i + 1; j < mSize; j++) {
			pResult[i] -= pMatrix[RowIndex][j] * pResult[j] / pMatrix[RowIndex][i];
		}
	}
	return 0;
}