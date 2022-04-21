#include "CGParallel.h"
#include "math.h"
#include <omp.h>


void CGParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 
	double *CurrentApproximation, *PreviousApproximation;
	double *CurrentGradient, *PreviousGradient;
	double *CurrentDirection, *PreviousDirection;
	double *Denom, *tempPointer;
	double Step;
	int Iter = 1, MaxIter = Size + 1;
	float Accuracy = 0.00001f; //шешім қателігі
	CurrentApproximation = new double[Size];
	PreviousApproximation = new double[Size];
	CurrentGradient = new double[Size];
	PreviousGradient = new double[Size];
	CurrentDirection = new double[Size];
	PreviousDirection = new double[Size];
	Denom = new double[Size];
	// Бастапқы мәндерді енгізіп қою
	for (int i = 0; i < Size; i++) {
		PreviousApproximation[i] = 0;
		PreviousDirection[i] = 0;
		PreviousGradient[i] = -pVector[i];
	}
	do {
		if (Iter > 1) {
			tempPointer = PreviousApproximation;
			PreviousApproximation = CurrentApproximation;
			CurrentApproximation = tempPointer;
			tempPointer = PreviousGradient;
			PreviousGradient = CurrentGradient;
			CurrentGradient = tempPointer;
			tempPointer = PreviousDirection;
			PreviousDirection = CurrentDirection;
			CurrentDirection = tempPointer;
		}

		// Градиентті есептеу
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			CurrentGradient[i] = -pVector[i];
			for (int j = 0; j < Size; j++)
				CurrentGradient[i] += pMatrix[i][j] * PreviousApproximation[j];
		}
		double IP1 = 0, IP2 = 0;

		// Бөлушектің алымы мен бөлімін алдын ала есептеп аламыз
#pragma omp parallel for reduction(+:IP1,IP2)
		for (int i = 0; i < Size; i++) {
			IP1 += CurrentGradient[i] * CurrentGradient[i]; // алымы
			IP2 += PreviousGradient[i] * PreviousGradient[i]; // бөлімі
		}

		// Бағыттауыш векторды есептеу
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			CurrentDirection[i] = -CurrentGradient[i] +
				PreviousDirection[i] * IP1 / IP2;
		}
		IP1 = 0;		IP2 = 0;
		// Бөлушектің алымы мен бөлімін алдын ала есептеп аламыз
#pragma omp parallel for reduction(+:IP1,IP2)
		for (int i = 0; i < Size; i++) {
			Denom[i] = 0;
			for (int j = 0; j < Size; j++)
				Denom[i] += pMatrix[i][j] * CurrentDirection[j];
			IP1 += CurrentDirection[i] * CurrentGradient[i];
			IP2 += CurrentDirection[i] * Denom[i];
		}
		// Қадам шамасын есептейміз
		Step = -IP1 / IP2;
		// Жаңа шешімді есептейміз
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			CurrentApproximation[i] = PreviousApproximation[i] + Step * CurrentDirection[i];
		}
		Iter++;
	} while /* алдыңғы шешім мен жаңа шешім айырмасын есептейміз  
			* егер ол қателіктен Accuracy үлкен болса
			* және қайталау саны массив өлшемінен кіші болса
			* жаңа градиент, бағыт, қадам есептейміз
			*/
		((diff(PreviousApproximation, CurrentApproximation, Size) > Accuracy)
			&& (Iter < MaxIter));
	for (int i = 0; i < Size; i++)
		pResult[i] = CurrentApproximation[i];
	iterationsCount = Iter;
}

// алдыңғы шешім мен жаңа шешім айырмасын есептейміз
double CGParallel::diff(double *vector1, double* vector2, int Size) {
	double sum = 0;
#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < Size; i++) {
		sum += fabs(vector1[i] - vector2[i]);
	}

	return sum;
}